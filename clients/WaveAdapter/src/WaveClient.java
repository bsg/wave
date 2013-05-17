import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

public class WaveClient {
	private Socket socket = null;
	private OutputStream os = null;
	private InputStream is = null;
	private WaveListener listener = null;

	public WaveClient(String host, int port) throws UnknownHostException,
			IOException {
		socket = new Socket(host, port);
		os = socket.getOutputStream();
		is = socket.getInputStream();
		socket.setSendBufferSize(1024 * 1024 * 32);
	}

	public void publish(int channel, String body) throws IOException {
		os.write(String.format("pub %d\n" + body + "\n", channel).getBytes());
	}

	public void subscribe(int channel, WaveListener listener)
			throws IOException {
		os.write(String.format("sub %d\n", channel).getBytes());
		this.listener = listener;
	}

	public void listen() {
		Thread thread = new Thread(new Runnable() {
			public void run() {
				byte[] buffer = new byte[256];
				
				while (true) {
					int nread = 0;
					try {
						/* TODO: Feed the stream into a parser */
						nread = is.read(buffer, 0, 256);
						if (nread != -1)
							listener.onReceive(new String(buffer));
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		});
		thread.start();
	}
}
