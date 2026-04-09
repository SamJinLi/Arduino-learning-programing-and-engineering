Imports OpenHardwareMonitor.Hardware
Imports System.IO
Imports System.IO.Ports
Imports System.ComponentModel
Imports RTSSSharedMemoryNET

Public Class FormMain

    Dim flag As Boolean
    ReadOnly cp As New Computer()

    ' Dim SerialPort1 As New SerialPort
    Dim ArduinoConnected As Boolean
    Dim RefreshTime As Integer
    '----------------------------------------
    'sensordata/sensordatastr values INDEX:
    '+|   0     |   1
    '------------------------
    '0|CpuTemp  | GpuTemp
    '------------------------
    '1|CpuLoad  | GpuLoad
    '------------------------
    '2|RAMLoad  | FPS
    '------------------------
    '3|MoBoTemp | EXT_SENSOR
    Dim SensorData(1, 3) As Integer
    Dim SensorDataStr(1, 3) As String
    '-----------------------------------------
    Dim HardwareNames(1, 1) As String
    'hardware name vars
    Dim CpuName(1) As String
    Dim GpuName(1) As String
    Dim SendNames As Boolean

    'CPU vars
    Dim CpuTemp As Integer
    Dim CpuLoad As Integer

    Dim CpuTempStr As String
    Dim CpuLoadStr As String

    'GPU vars
    Dim GpuTemp As Integer
    Dim GpuLoad As Integer
    Dim Fps As Integer

    Dim GpuTempStr As String
    Dim GpuLoadStr As String
    Dim FpsStr As String

    'RAM vars
    Dim RAMLoad As Integer

    Dim RAMLoadStr As String

    'MoBo vars
    Dim MoboTemp As Integer
    Dim MoBoTempStr As String

    Private Sub FormMain_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        Dim AtBoot As Boolean = False
        For Each st In My.Computer.Registry.CurrentUser.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", False).GetValueNames
            If st = Application.ProductName Then
                AtBoot = True
            End If
        Next

        ToolStripStartAtBoot.Checked = AtBoot

        'Me.WindowState = FormWindowState.Minimized
        ToolStripComboBoxRefreshTime.SelectedIndex = My.Settings.RefreshId
        ToolStripMenuAutoconnect.Checked = My.Settings.AutoConnect
        RefreshTime = (My.Settings.RefreshId + 1) * 1000 '= (ToolStripComboBoxRefreshTime.SelectedIndex + 1) * 1000
        LblRefresh.Text = "Refresh time: " + CStr(RefreshTime) + " milliseconds"
        Timer1.Interval = RefreshTime
        Timer1.Start()
        cp.GPUEnabled = True
        cp.CPUEnabled = True
        cp.RAMEnabled = True
        cp.MainboardEnabled = True
        'cp.FanControllerEnabled = True

        cp.Open()
        Fps = 0

        'retrieve components name, just once
        For Each hw In cp.Hardware
            Select Case hw.HardwareType
                Case HardwareType.CPU
                    HardwareNames(0, 0) = FormatNames(hw.Name)(0)
                    HardwareNames(0, 1) = FormatNames(hw.Name)(1)
                Case HardwareType.GpuNvidia
                    HardwareNames(1, 0) = FormatNames(hw.Name)(0)
                    HardwareNames(1, 1) = FormatNames(hw.Name)(1)
            End Select
        Next
        For i As Integer = 0 To 1

            ListBox1.Items.Add(HardwareNames(0, i))
            ListBox2.Items.Add(HardwareNames(1, i))

        Next
        SendNames = True

    End Sub


    Private Sub Autoconnect()

        For Each sp As String In My.Computer.Ports.SerialPortNames
            Try
                SerialPort1.PortName = sp
                SerialPort1.BaudRate = 9600
                SerialPort1.DataBits = 8
                SerialPort1.Parity = Parity.None
                SerialPort1.StopBits = StopBits.One
                SerialPort1.Handshake = Handshake.None
                SerialPort1.Encoding = System.Text.Encoding.Default
                SerialPort1.ReadTimeout = 1000
                SerialPort1.WriteTimeout = 1000
                SerialPort1.Open()

                Dim ListeningWatch As New Stopwatch
                Dim serialMessage As String

                SerialPort1.Write("*****;")

                ListeningWatch.Start()
                While ListeningWatch.ElapsedMilliseconds < 3000 And ListeningWatch.IsRunning
                    serialMessage = SerialPort1.ReadLine()
                    Label1.Text = serialMessage
                    If serialMessage.Contains("R") Then
                        ListeningWatch.Stop()
                        ConnMenuToolStripMenuItem.Text = "Disconnect"
                        ArduinoConnected = True
                        Label1.Text = "Arduino Connected " + sp
                        flag = True
                        SendNames = True
                        Exit For
                    End If
                End While
            Catch ex As TimeoutException
                ArduinoConnected = False
                Exit Try
            Catch ey As InvalidOperationException
                ArduinoConnected = False
                Exit Try
            Catch ez As IOException
                ArduinoConnected = False
                Exit Try
            End Try

        Next
        If ArduinoConnected = False Then
            SerialPort1.Close()
        End If
    End Sub

    Private Sub SendDataToArduino()
        Dim OutputString(3) As String
        'commands:
        '0: send names
        '1: send cpu data and case temp
        '2: send gpu data
        '3: print data

        If SendNames Then
            OutputString(0) = "0:"
            For i As Integer = 0 To 3
                OutputString(0) = OutputString(0) + HardwareNames(i \ 2, i Mod 2) + ","
            Next
            OutputString(0) = OutputString(0) + ";"
        Else
            OutputString(0) = ""
        End If

        'add sections
        For i As Integer = 1 To 2
            OutputString(i) = CStr(i) + ":"
            For ii As Integer = 0 To 3
                OutputString(i) = OutputString(i) + SensorDataStr(i - 1, ii) + ","
            Next
            OutputString(i) = OutputString(i) + ";"
        Next
        OutputString(3) = "3:;"

        ' While SerialPort1.IsOpen
        Try
            If SendNames Then
                SerialPort1.WriteLine(OutputString(0))
                SerialPort1.WriteLine(OutputString(3))
                SendNames = False
            End If
            For i As Integer = 1 To 3
                SerialPort1.Write(OutputString(i))
            Next
        Catch ex As InvalidOperationException
            Me.ConnMenuToolStripMenuItem.PerformClick()
        End Try
    End Sub


    Private Function FormatValue(InValue As Integer) As String
        'TypeFormat values:1=temp,2=load,3=fps
        Dim OutStr As String
        'add spaces to numeric strings, or set to "---" for invalid values
        Select Case InValue
            Case 0 To 9
                OutStr = "  " + CStr(InValue)
            Case 10 To 99
                OutStr = " " + CStr(InValue)
            Case 100 To 998
                OutStr = CStr(InValue)
            Case Else
                OutStr = "---"
        End Select

        Return OutStr
    End Function

    Private Function FormatNames(InValue As String) As String()
        Dim OutValue(1) As String
        Dim tempValue(1) As String
        Dim spaceIndex(0) As Integer
        Dim spaceCount As Integer
        Dim spacePos As Integer
        spaceCount = 0
        spacePos = 0
        'find all the spaces and make an index
        For Each c As Char In InValue
            If c = " " Then
                ReDim Preserve spaceIndex(spaceCount + 1)
                spaceIndex(spaceCount) = spacePos
                spaceCount += 1
            End If
            spacePos += 1
        Next
        Dim strLenHalf = Int(InValue.Length / 2)
        Dim spaceDistance As Integer
        Dim nearestSpace As Integer = InValue.Length
        Dim middleSpace As Integer
        For i As Integer = 0 To spaceCount
            spaceDistance = Math.Abs(spaceIndex(i) - strLenHalf)
            If spaceDistance <= nearestSpace Then
                nearestSpace = spaceDistance
                middleSpace = i
            End If
        Next
        OutValue(0) = InValue.Substring(0, spaceIndex(middleSpace)).Trim
        OutValue(1) = InValue.Substring(spaceIndex(middleSpace)).Trim
        For ii As Integer = 0 To 1
            Do Until OutValue(ii).Length = 14
                If OutValue(ii).Length Mod 2 = 0 Then
                    OutValue(ii) = OutValue(ii) + " "
                Else
                    OutValue(ii) = " " + OutValue(ii)
                End If
            Loop
        Next
        Return OutValue
    End Function

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        Timer1.Interval = (My.Settings.RefreshId + 1) * 1000
        If ToolStripMenuAutoconnect.Checked = True And ArduinoConnected = False Then
            ConnMenuToolStripMenuItem.PerformClick()
        End If
        For Each hw In cp.Hardware
            hw.Update()
            Select Case hw.HardwareType
                Case HardwareType.CPU   'CPU stuff
                    For Each sensor In hw.Sensors
                        Select Case sensor.SensorType
                            Case SensorType.Temperature
                                If sensor.Name.Contains("Package") Then
                                    SensorData(0, 0) = sensor.Value ' CpuTemp
                                End If
                            Case SensorType.Load
                                If sensor.Name.Contains("Total") Then
                                    SensorData(0, 1) = sensor.Value 'CpuLoad
                                End If
                        End Select
                    Next
                Case HardwareType.GpuNvidia   'GPU stuff
                    For Each sensor In hw.Sensors
                        Select Case sensor.SensorType
                            Case SensorType.Temperature
                                SensorData(1, 0) = sensor.Value 'GpuTemp
                            Case SensorType.Load
                                If sensor.Name.Contains("Core") Then
                                    SensorData(1, 1) = sensor.Value 'GpuLoad
                                End If
                        End Select
                    Next
                Case HardwareType.RAM 'RAM stuff
                    For Each sensor In hw.Sensors
                        If sensor.SensorType = SensorType.Load Then
                            SensorData(0, 2) = sensor.Value 'RAMLoad
                        End If
                    Next
                Case HardwareType.Mainboard   'MoBo stuff
                    For Each subHard In hw.SubHardware
                        subHard.Update()
                        For Each sensor In subHard.Sensors
                            If sensor.SensorType = 2 And sensor.Name.Contains("#2") Then
                                SensorData(0, 3) = sensor.Value 'MoboTemp
                            End If
                        Next
                    Next
            End Select
        Next

        'FPS
        Dim appEntries = OSD.GetAppEntries()
        SensorData(1, 2) = 0
        For Each app In appEntries
            If app.InstantaneousFrames > 0 Then
                SensorData(1, 2) = CInt(app.InstantaneousFrames) 'FPS
            End If
        Next
        If SensorData(1, 2) < 2 Then 'if FPS<2 there is no 3d app running, then set FPS to 999 so FPS string will be "---"
            SensorData(1, 2) = 999
        End If

        'EXT_SENSOR (set to fixed value, managed by arduino)
        SensorData(1, 3) = 999

        'formats all the values in strings.
        For i As Integer = 0 To 1
            For ii As Integer = 0 To 3
                SensorDataStr(i, ii) = FormatValue(SensorData(i, ii))
            Next
        Next

        'write data in listboxes
        ListBox1.Items.Clear()
        ListBox2.Items.Clear()
        'names
        For i As Integer = 0 To 1
            ListBox1.Items.Add(HardwareNames(0, i))
            ListBox2.Items.Add(HardwareNames(1, i))
        Next
        'values
        For i As Integer = 0 To 3
            ListBox1.Items.Add(SensorDataStr(0, i))
            ListBox2.Items.Add(SensorDataStr(1, i))
        Next


        If flag = True Then

            SendDataToArduino()

        End If
    End Sub

    Private Sub NotifyIcon1_MouseDoubleClick(sender As Object, e As MouseEventArgs) Handles NotifyIcon1.MouseDoubleClick
        Me.MinimMaximMenuToolStripMenuItem.PerformClick()
    End Sub

    Private Sub FormMain_SizeChanged(sender As Object, e As System.EventArgs) Handles Me.SizeChanged
        Select Case Me.WindowState
            Case FormWindowState.Minimized
                MinimMaximMenuToolStripMenuItem.Text = "Maximize"
            Case FormWindowState.Normal
                MinimMaximMenuToolStripMenuItem.Text = "Minimize"
        End Select

    End Sub

    Private Sub ConnMenuToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ConnMenuToolStripMenuItem.Click
        If ArduinoConnected Then
            flag = False
            SerialPort1.Close()
            ConnMenuToolStripMenuItem.Text = "Connect"
            Label1.Text = "Disconnected"
            ArduinoConnected = False
        Else
            Autoconnect()
        End If
    End Sub

    Private Sub ExitToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles ExitToolStripMenuItem.Click
        Me.Close()
    End Sub

    Private Sub MinimMaximMenuToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles MinimMaximMenuToolStripMenuItem.Click
        Select Case Me.WindowState
            Case FormWindowState.Minimized
                Me.WindowState = FormWindowState.Normal
            Case FormWindowState.Normal
                Me.WindowState = FormWindowState.Minimized
        End Select
    End Sub

    Private Sub ToolStripMenuAutoconnect_Click(sender As Object, e As EventArgs) Handles ToolStripMenuAutoconnect.Click
        ToolStripMenuAutoconnect.Checked = Not (ToolStripMenuAutoconnect.Checked)
        My.Settings.AutoConnect = ToolStripMenuAutoconnect.Checked
    End Sub

    Private Sub ToolStripComboBoxRefreshTime_SelectedIndexChanged(sender As Object, e As EventArgs) Handles ToolStripComboBoxRefreshTime.SelectedIndexChanged
        My.Settings.RefreshId = ToolStripComboBoxRefreshTime.SelectedIndex
    End Sub

    Private Sub ToolStripStartAtBoot_Click(sender As Object, e As EventArgs) Handles ToolStripStartAtBoot.Click
        If ToolStripStartAtBoot.Checked Then
            'disable atboot
            Dim taskDelCommand As String = "schtasks /delete /f /tn " + """Apps\PcHwMon_server"""
            My.Computer.Registry.CurrentUser.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", True).DeleteValue(Application.ProductName)
            Shell(taskDelCommand,, True)
            ToolStripStartAtBoot.Checked = False
        Else
            My.Computer.Registry.CurrentUser.OpenSubKey("SOFTWARE\Microsoft\Windows\CurrentVersion\Run", True).SetValue(Application.ProductName, Environment.SystemDirectory + "\schtasks.exe /run /tn ""Apps\pcHwMon_server""")
            ToolStripStartAtBoot.Checked = True
            Dim fileContent As String
            Dim fileWriter As StreamWriter
            Dim taskCommandLine As String = "schtasks /create /xml " + """" + Application.StartupPath + "\temp\pcHwMon_server.xml" + """" + " /tn " + """Apps\PcHwMon_server"""
            My.Computer.FileSystem.CreateDirectory(Application.StartupPath + "\temp")
            fileContent = My.Resources.Resource1.String1 + My.User.Name +
                My.Resources.Resource1.String2 + System.Security.Principal.WindowsIdentity.GetCurrent().User.ToString +
                My.Resources.Resource1.String3 + Application.ExecutablePath +
                My.Resources.Resource1.String4
            fileWriter = New StreamWriter(Application.StartupPath + "\temp\pcHwMon_server.xml")
            fileWriter.Write(fileContent)
            fileWriter.Close()
            Shell(taskCommandLine,, True)
            My.Computer.FileSystem.DeleteDirectory(Application.StartupPath + "\temp", FileIO.DeleteDirectoryOption.DeleteAllContents)
        End If
        My.Computer.Registry.CurrentUser.Close()

    End Sub
End Class
