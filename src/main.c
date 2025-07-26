//
//  main.c
//  wav
//
//  Created by Kirk Roerig on 7/25/25.
//  
//  Minimal example demonstrating the Playdate SDK's sound API for basic
//  sound effect playback.
//

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"

PlaydateAPI* pd = NULL;
const struct playdate_sound* snd = NULL;

AudioSample* s = NULL;
SamplePlayer* sp = NULL;

int update(void* ud);

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	(void)arg;
	
	if ( event == kEventInit )
	{
		// Convenience for using the Playdate API
		pd = playdate;
		snd = playdate->sound;
		
		// Load the .wav, .wave files are automatically converted to .pda which is loaded without an extension
		s = snd->sample->load("bark");
		if (!s) { pd->system->error("Error loading sample: %s", snd->getError()); }

		// Create a sample player to play the sample we loaded above
		sp = snd->sampleplayer->newPlayer();
		if (!sp) { pd->system->error("Error creating sample player: %s", snd->getError()); }
		snd->sampleplayer->setSample(sp, s);
		snd->sampleplayer->setVolume(sp, 0.5, 0.5);

		pd->system->setUpdateCallback(update, NULL);
	}
	
	return 0;
}

int
update(void* ud)
{
	(void)ud;

	PDButtons current, pressed, released;
	pd->system->getButtonState(&current, &pressed, &released);

	if (pressed & kButtonA)
	{
		pd->system->logToConsole("**bark sample**");
		snd->sampleplayer->play(
			sp, // Sample player pointer 
			1,  // Times to repeat sample
			1.f // Playback rate. 1 is normal, 0.5 is half speed, etc
		);
	}

	return 1;
}
