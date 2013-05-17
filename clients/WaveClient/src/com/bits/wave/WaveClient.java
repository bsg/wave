package com.bits.wave;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

/**
 * Wave connector.
 * 
 * @author Cem Saldırım
 * @version 0.1
 *
 */
public class WaveClient {
	private Socket socket = null;
	private OutputStream os = null;
	private InputStream is = null;
	private WaveListener listener = null;

	/**
	 * Establishes a connection to wave
	 * 
	 * @param host Broker host
	 * @param port Broker port
	 * @throws UnknownHostException Self-explanatory
	 * @throws IOException Connection fuxored.
	 */
	public WaveClient(String host, int port) throws UnknownHostException,
			IOException {
		socket = new Socket(host, port);
		os = socket.getOutputStream();
		is = socket.getInputStream();
	}

	/**
	 * Publish a message to a channel
	 * 
	 * @param channel Channel ID
	 * @param body Message body
	 * @throws IOException Connection fuxored.
	 */
	public void publish(int channel, String body) throws IOException {
		os.write(String.format("pub %d\n" + body + "\n", channel).getBytes());
	}

	/**
	 * Subscribe to a channel
	 * 
	 * @param channel Channel ID
	 * @param listener Reference to a WaveListener implementation
	 * @throws IOException Connection fuxored.
	 */
	public void subscribe(int channel, WaveListener listener)
			throws IOException {
		os.write(String.format("sub %d\n", channel).getBytes());
		this.listener = listener;
	}

	private String byteListToString(List<Byte> l, Charset charset) {
		if (l == null) {
			return "";
		}

		byte[] array = new byte[l.size()];
		int i = 0;

		for (Byte current : l) {
			array[i] = current;
			i++;
		}

		return new String(array, charset);
	}

	/**
	 * Start listening for incoming messages
	 */
	public void listen() {
		Thread thread = new Thread(new Runnable() {
			public void run() {
				byte c;
				List<Byte> body = new ArrayList<Byte>();
				List<Byte> ch = new ArrayList<Byte>();

				while (true) {
					body.clear();
					ch.clear();
					try {
						/* Ugly but works */
						/* TODO: Profile */
						if (is.read() == 'm') {
							if (is.read() == 's') {
								if (is.read() == 'g') {
									if (is.read() == ' ') {
										/* Get channel id */
										while ((c = (byte) is.read()) != '\n') {
											ch.add(c);
										}
										/* Get body */
										while ((c = (byte) is.read()) != '\n') {
											body.add(c);
										}
										listener.onReceive(Integer.parseInt(byteListToString(ch, Charset
												.forName("utf-8"))),
												byteListToString(body, Charset
														.forName("utf-8")));
									}
								}
							}
						}
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
