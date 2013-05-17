import java.io.IOException;
import java.net.UnknownHostException;

public class Test implements WaveListener {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
//		int ch;
//		for(ch = 1; ch < 2; ch++)
//			addRaepist(ch % 256);
		raep(1);
	}
	
	public static void addRaepist(final int ch) {
		Thread thread = new Thread(new Runnable() {
			@Override
			public void run() {
				raep(ch);
			}
		});
		
		thread.start();
	}
	
	public static void raep(int ch) {
		WaveClient waveAdapter = null;
		try {
			waveAdapter = new WaveClient("localhost", 9999);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return;
		}

		
//		Integer i = 0;
//		while (i < 1024 * 1024 * 1024) {
//			try {
//				Thread.sleep(1000);
//				waveAdapter.publish(ch, "test " + i++);
//			} catch (IOException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			} catch (InterruptedException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//		}

		try {
			waveAdapter.subscribe(1, new ChannelListener());
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		waveAdapter.listen();

	}

	@Override
	public void onReceive(String body) {
		System.out.println(body);
	}
}
