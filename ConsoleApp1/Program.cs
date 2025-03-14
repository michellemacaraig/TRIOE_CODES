using System;
using System.IO.Ports;

class CH9329Controller
{
    static void Main()
    {
        SerialPort serialPort = new SerialPort("COM3", 9600, Parity.None, 8, StopBits.One);
        
        try
        {
            serialPort.Open();
            Console.WriteLine("Connected to CH9329");

            byte[] command = { 0x57, 0xAB, 0xCD }; // Example HID command
            serialPort.Write(command, 0, command.Length);
            Console.WriteLine("Command Sent!");

            serialPort.Close();
        }
        catch (Exception e)
        {
            Console.WriteLine("Error: " + e.Message);
        }
    }
}
