


s16 = read_audio_sample(); // get current input sample
// cycle to next number in buffer
writeIndex = (writeIndex + SIZE_OF_BUFFER - 1) % SIZE_OF_BUFFER;

// implement distortion algorithm
if(dist){
	// shift audio file for asymmetry
	buffer[writeIndex] = (gain*s16)+0.5;
	// gain stage 1
	if(buffer[writeIndex] > 2/3 || buffer[writeIndex] < -2/3){
	buffer[writeIndex] = buffer[writeIndex]/(4*gain);
	}
}

// processing
outputSample = buffer[writeIndex];
write_audio_sample(outputSample); // output audio


