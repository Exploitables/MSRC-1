# MSRC Case 1
 Microsoft seemingly closed my MSRC account, and as such I will be releasing this case publicly.
 
# Case Report
 This write-up is formatted in Zero Day Initiative's report format.

1.	Microsoft Windows Tunnel Interface Driver TunnelDriverDispatchDevControl Null Pointer Dereference Denial of Service Vulnerability

2.	A vulnerability exists in the Microsoft Windows Tunnel Interface Driver when it improperly validates a pointer prior to using it. An attacker can effectively shut down the affected system, or potentially disclose sensitive kernel memory. An attacker must have low-privileged access on the machine to exploit this vulnerability.

3.	Microsoft Windows 10 Pro Version 21H2 (OS Build 19044.1288), Microsoft Tunnel Interface Driver 'tunnel.sys' File version 10.0.19041.1, Product version 10.0.19041.1

4.	The root cause appears to be that the device driver does not validate if the pointer is valid, thus leading to a null pointer dereference. First and foremost, to interact with the driver, we must obtain a handle to the device object (in this scenario, '\\.\GLOBALROOT\Device\TunnelControl'). After sending an IO control code request, the driver first compares an IO control code that is sent to it against the number 4, and acts accordingly. To hit the target code path, this check must be failed. To fail the check, the only requirement is that we pass in an integer that is not equal to 4. After failing this check, the driver drops down to a chain of instructions. These instructions only manipulate registers, and are presumably for returning from the function. The targeted instruction in this chain is `mov rax, qword ptr [rcx+20h]', without verifying that the data pointed to by 'rcx+20h' is valid. As a result, the provided proof-of-concept code will trigger the vulnerability such that 'rcx' will contain zero, thus dereferencing a null pointer. As it does not appear that the data is returned back to the attacker, this can only be used as an effective denial of service against machines with this service enabled. It is unclear as to how to directly control the contents of the 'rcx' register. It is also unclear as to how any IO control code passed to the device driver (except for 0) does not crash the target device. A solution to this vulnerability is to validate the contents of these registers prior to using them, specifically the 'rcx' register.

5.	The proof-of-concept solution has been provided in the attached zip. The tunnel interface driver does not run on Windows by default, and thus must be enabled. To enable this device driver, install Process Hacker 2 (https://sourceforge.net/projects/processhacker/files/processhacker2/processhacker-2.39-setup.exe/download), and traverse to the "Services" tab at the top of the Process Hacker window. From here, search for the device driver name "tunnel" and the driver will appear. Right click the device driver and click "Start" to start the service. The only step afterwards is to execute the proof-of-concept code on the machine.

6.	Microsoft Windows 10 Download Link: https://www.microsoft.com/en-us/software-download/windows10
