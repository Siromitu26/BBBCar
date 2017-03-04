package main;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class TCPClient {

	public static void main(String[] args) {
		// TODO 自動生成されたメソッド・スタブ
		System.out.println("Hello world\n");
		String hostname = "BBBCar";
		int port  = 12345;
		String str = new String();
		int data = 0;
		try {
			System.out.println("Connecting...");
			Socket socket = new Socket(hostname, port);
			System.out.println("Connected to " + socket.getInetAddress().toString());
			DataOutputStream out = new DataOutputStream(socket.getOutputStream());
			int i = 0;
			boolean flag = true;
			while(flag){
				 	data =  createCarControlData();
				 	if(((data >> 23) & 1) == 1) flag = false;
					System.out.println("Send:" + data);
					byte[] bs = new byte[4];
					bs[0] = (byte) (0x000000ff & (data));
					bs[1] = (byte) (0x000000ff & (data >>> 8));
					bs[2] = (byte) (0x000000ff & (data >>> 16));
					bs[3] = (byte) (0x000000ff & (data >>> 24));
					try {
						out.write(bs, 0, 4);
						out.flush();
					}catch(Exception e){
						flag = false;
					}
			}
			System.out.println("Last Number:" + data);
			socket.close();
		}catch(Exception e){
			e.printStackTrace();
		}
		return;
	}


	public static int createCarControlData(){
		int result = 0;
		int temp = 0;
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String[][] str = {
				{"Steering", "0:直進", "1:左折", "2:右折"},
				{"sPwm", "Percentage"},
				{"Drive", "0:フリー", "1:前進", "2:後退", "3:ブレーキ"},
				{"dPwm", "Percentage"},
				{"EndFlag", "0 or 1"}
		};
		for(int i = 0; i < str.length; i++){
			System.out.println("Type your value to " + str[i][0] + ".");
			for(int j = 0; j < str[i].length; j++){
				System.out.print(str[i][j]);
			}
			try {
				switch(i){
				case 0:
					temp = Integer.parseInt(br.readLine()) & 3;
					break;
				case 1:
					temp = (Integer.parseInt(br.readLine()) & 255) << 4;
					break;
				case 2:
					temp = (Integer.parseInt(br.readLine()) & 3) << 2;
					break;
				case 3:
					temp = (Integer.parseInt(br.readLine()) & 255) << 12;
					break;
				case 4:
					temp = (Integer.parseInt(br.readLine()) & 1) << 24;
					break;
				}
			}catch(NumberFormatException e1){
				temp = 0;
			}catch(IOException e2){
				temp = 0;
			}
			result += temp;
		}
		return result;
	}
}
