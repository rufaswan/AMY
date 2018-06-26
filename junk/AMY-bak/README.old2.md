AMY
======

Awesome Multimedia plaYground

!! for map use vector <tile>
!! use reserve( w*h ) to preallocate the memory (and to prevent dynamic reallocate)
!! make int getmapindex(int x, int y) { return (y*w)+x; }
!! for command list, use deque
!! use push_front( in )
!! make bool is_hold( in ) { if ( deque[0] == in && deque[1] == in ) return true else false; }

class time
{
	int fps
	Int32 prev_msec
	Int32 interval
	Int32 timer

	void setfps(int fps)
		time::fps = fps
		time::interval = 1000 / fps

	bool can_update()
		// get current time in minisecond
		Int32 msec = ???
		time::timer = time::timer + ( msec - time::prev_msec )
		time::prev_msec = msec
		if ( time::timer > time::interval )
			time::timer = time::timer - time::interval
			return true
		else
			return false
}

stage( $replay_data )
(
	function key_input
		player->exec_commmand(key)

	funtion run
		while run
			player->update_animation
			player->draw
			foreach ( enemies as $e )
				$e->update
				$e->draw

	function replay_run
		i = 0
		while replay data
			player->exec_command( frame[i] )
			i++
)

char ()
{
	timeout
	prev_cmd1
	prev_cmd2

	exec command( key )
	if key == 0
		cmd = walkleft

	function is cmd end()
		if cmd != ''
			if cmd = walkleft
		else
			exec command ( random cmd_list() )

	update

	update animation

	reset

}

particle(x, y)
{
	timeout = 40 fps;
	draw()
}
STATE.particle draw loop
if timeout < 0, removed

void Stage::update_player(int replay)
{
	if ( replay > 0 )
	{
		if ( replay & KEY_UP )		STATE.PLAYER->update();
		if ( replay & KEY_DN )		STATE.PLAYER->update();
		if ( replay & KEY_LF )		STATE.PLAYER->update();
		if ( replay & KEY_RT )		STATE.PLAYER->update();
		if ( replay & KEY_STR )		STATE.PLAYER->update();
		if ( replay & KEY_SEL )		STATE.PLAYER->update();
		if ( replay & KEY_L_TR )	STATE.PLAYER->update();
		if ( replay & KEY_R_TR )	STATE.PLAYER->update();
		if ( replay & KEY_DSH )		STATE.PLAYER->update();
		if ( replay & KEY_JMP )		STATE.PLAYER->update();
		if ( replay & KEY_SHT )		STATE.PLAYER->update();
		if ( replay & KEY_RPD )		STATE.PLAYER->update();
	}
	else
	{
		if ( STATE.KEYS[ KEY_UP ] )		STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_DN ] )		STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_LF ] )		STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_RT ] )		STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_STR ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_SEL ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_L_TR ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_R_TR ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_DSH ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_JMP ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_SHT ] )	STATE.PLAYER->update();
		if ( STATE.KEYS[ KEY_RPD ] )	STATE.PLAYER->update();
	}
}

fighting game input system
- no delay, accept input on every frame
- on HOLD, the button is register once
- on HOLD, count how long the key holded (for Guile's Sonic Boom)
- command checking happens only when P or K is pressed, not direction button
- check also the distant for throws moves
- on NO_KEY_PRESS, the new input is check against the queue for command exec
- on NO_KEY_PRESS, for longer than 1 sec, the queue is clear
- on NO_KEY_PRESS, but on action, the queue is NOT clear until the move is finished
- on crouch, the keys is NOT cleared until (key released) the char stand still and do nothing for 1 sec
- the queue holds 15 input (as on display), but can be more
- corner keys are seperate, no mixed (don't register as HOLD)
- each input can mixed button, e.g. KKK, ( <- K), ( -> PPP )
- input   -> PPP   mean press (and hold) ->, then press PPP (within frames, true for most moves)
-         can also mean press and release ->, then press PPP (within frames, true for LPLP->LKHP type moves)
- input ( -> PPP ) mean press -> and PPP at the same time
- spinning bird kick can be exec | \ - K , or | ( \ K ) -
  read : DCF K or D (C K) F
- key up and down will cancel each other
- key left and right will cancel each other
- when K is press, check the queue for command exec
- can check for queue lenght first for shortlisting the commands to compare

timeline animation system
- table like structure
- time as column
- object as row
- row on top is drawn first, making them BackGround layer
- row on bottom is drawn last, making them the ForeGround layer
- each cell should have (at least)
-- pos_x and pos_y for positioning
-- flip_x or flip_y for facing
-- the tile to draw

particle system
- should have at least
-- the tile to draw
-- the time to appear
-- the time to last



main
	|- GameMaster.init()
	|- GameMaster.run()
		|- while 1
			|- GameMaster::run_level( lvl )
				|- display = new Stage
				|   |- *display
				|
				|- display->run()
					|- while 1
						|- display.handles()
						|   |- (DATA) Controller.handle_input()
						|- *display.updates()
						|- display.display()

recording performace (N 60fps)
only add to vbuf, no process / fwrite yet
1/60  12   2/30  20   3/20  27   4/15  32
5/12  35   6/10  36   7/8.  39   8/7.  40
9/6.  45

10/6  45  20/3  52  30/2  56  40/1. 56
50/1. 57  60/1  57

        self.objects = gameobject.Group()
        self.coins = gameobject.Group()
        self.baddies = gameobject.Group()
        self.dead = gameobject.Group()
        self.springs = gameobject.Group()

        Player.groups = [self.objects]
        Platform.groups = [self.objects]
        Coin.groups = [self.objects, self.coins]
        Points.groups = [self.objects]
        Poof.groups = [self.objects]
        Baddie.groups = [self.objects, self.baddies]
        Death.groups = [self.objects, self.dead]
        Spring.groups = [self.objects, self.springs]

        for object in self.objects:
            if (object.rect != self.camera.left):
                object.update(self.engine.tiles)

        # Get rich quick!
        for c in self.coins:
            if self.player.rect.colliderect(c.rect):
                # process

        # Will you live, or die?
        for b in self.baddies:
            if self.player.rect.colliderect(b.rect):
                # process

        # Boinnnng!
        for s in self.springs:
            if self.player.rect.colliderect(s.rect):
                # process


    objects = retrogamelib.gameobject.Group()
    platforms = retrogamelib.gameobject.Group()
    bullets = retrogamelib.gameobject.Group()

    Bullet.groups = [objects, bullets]
    Player.groups = [objects]
    Player.platforms = platforms
    Player.objects = objects
    Platform.groups = [objects, platforms]
    Tile.groups = [objects]
    Bullet.platforms = platforms

6 types of entities
1 player char    2 enemy char
3 player bullet  4 enemy bullet
5 platform       6 particles

1-2 player damaged
1-3 nil
1-4 player damaged
1-5 stand/move according to platform
2-3 enemy damaged
2-4 nil
2-5 stand/move according to platform
3-4 nil
3-5 nil
4-5 nil

n1 : particles does not need to check for collusion. Animation end = kill
n2 : enemy char & enemy bullets will damage player char
n3 : only player bullets can damage enemy char
n4 : both player char & enemy char will stand/move along with platform

animations
anm.add("stand", 5,   3,4,5,6,7);
anm.set_loop("stand");
anm.set_default("fall");

EnGroup
|- Entity (new Valkyrie) (type player)
	|- EnData (dmgbox)
	|- EnData (hitbox) (sword strike)
	|- EnData (sword wave)
	|- EnData (particles)
|- Entity (new Bat) (type enemy)
	|- EnData (bat1)(front formation)
	|- EnData (bat2)(middle formation)
	|- EnData (bat3)(back formation)
|- Entity
	|- EnData
	|- EnData
	|- EnData
	|- EnData
|- Entity
	|- EnData
	|- EnData
	|- EnData
	|- EnData

Entity : base_x and base_y; (absolute)
EnData : pos_x + base_x and pos_y + base_y; (relative)

EnGroup EG;
EG.add( new Valkyrie(x, y), "player" );
EG.add( new Bat(x, y), "en" );
EG.update()
-->
	// update all objects
	for ( i=0; i < EG.entity.size(); i++ )
		EG.entity[i].update()
		-->
			dead = true
			for ( i=0; i < Entity.endata.size(); i++ )
				if i == player
					Entity.endata[i].update()
				else
					Entity.endata[i].update_npc()
				-->
					Endata //

				// signal for removal
				// if one of the endata is still alive, the entity is not dead
				if ( ! Entity.endata[i].dead )
					dead = false

	// collusion check
	// check only the untested pairs
	// e.g. check between 1,2,3,4,5
	//      = 1-2, 1-3, 1-4, 1-5, 2-3, 2-4, 2-5, 3-4, 3-5, 4-5
	//      = 10 pairs
	for ( i=0; i < EG.entity.size(); i++ )
		for ( j=i+1; j < EG.entity.size(); j++ )
			EG.entity[i].collusion( EG.entity[j] )
			-->
				for ( i=0; i < Entity.endata.size(); i++ )
					for ( j=0; j < en.size(); j++ )
						Entity.endata[i].collusion( en[j] )
						-->
							if ( EnData.type == particle ) return
							if ( EnData.rect.empty() ) return




unsigned int to int(uint ui, uint bit)
	uint max = 2 ^ bit;

	// 1 byte = 8-bit = 256
	// OR = 0 to 255 = -128 to 127
	if ui >= max
		return 0

	if ui < ( max / 2 )
		return ui
	else
		return ui - max
