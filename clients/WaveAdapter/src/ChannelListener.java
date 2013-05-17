
public class ChannelListener implements WaveListener {

	@Override
	public void onReceive(String body) {
		System.out.println(body);
	}
}
