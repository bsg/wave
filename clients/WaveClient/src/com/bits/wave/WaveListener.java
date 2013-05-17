package com.bits.wave;

/**
 * Message listener interface.
 * 
 * @author Cem Saldırım
 *
 */
public interface WaveListener {
	/**
	 * Callback handler.
	 * 
	 * @param channel Channel ID of the message
	 * @param body Message body
	 */
	abstract void onReceive(int channel, String body);
}
