import java.net.Socket;
import java.io.DataInputStream;
import java.io.DataOutputStream;

public class TestClient {
    public static void main(String[] args) throws java.io.IOException {
        Socket sock = new Socket("localhost", 25565);
        DataOutputStream dos = new DataOutputStream(sock.getOutputStream());
        DataInputStream dis = new DataInputStream(sock.getInputStream());
        dos.writeByte(0xFE);
        if (dis.readUnsignedByte() != 0xFF) {
            System.err.println("Invalid packet header returned!");
            return;
        }
        int length = dis.readUnsignedShort();
        System.out.println("String length = " + length);
        char[] chars = new char[length];
        for (int i=0; i<length; ++i) {
            chars[i] = dis.readChar();
        }
        String motd = new String(chars);
        System.out.println("Recieved: " + motd);
        for (int i=0; i<chars.length; ++i) {
            System.out.println("chars[" + i + "] = " + chars[i] + ", binary = " + Integer.toBinaryString(chars[i]));
        }
    }
}
