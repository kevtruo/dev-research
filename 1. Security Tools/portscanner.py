import socket

print("TCP or UDP?")
protocol = input().upper()

if protocol == "TCP":
    print("TCP selected")
    print("Enter target IP:")
    target = input("> ")
    print("Scanning target " + target)
    for port in range(1, 1025):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1)  # Optional: Set a timeout to avoid hanging
        result = s.connect_ex((target, port))
        if result == 0:
            print("Port: " + str(port) + " Open")
        s.close()

elif protocol == "UDP":
    print("UDP selected")
    print("Enter IP:")
    target = input("> ")
    print("Scanning " + target)
    for port in range(1, 1025):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.settimeout(1)  # Optional: Set a timeout to avoid hanging
        try:
            s.sendto(b'', (target, port))
            data, _ = s.recvfrom(1024)
            print("Port: " + str(port) + " Open")
        except socket.timeout:
            pass  # No response, likely closed port
        except Exception as e:
            pass  # Handle other potential exceptions
        finally:
            s.close()

else:
    print("Invalid selection. Please choose TCP or UDP")
