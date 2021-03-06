#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleEnd.h"

ModuleInitScene::ModuleInitScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	table = NULL;

}

ModuleInitScene::~ModuleInitScene()
{}

// Load assets
bool ModuleInitScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->endScene->Disable();
	App->renderer->camera.x = App->renderer->camera.y = 0;

	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	bluefx = App->audio->LoadFx("pinball/bluesensor.wav");
	combofx = App->audio->LoadFx("pinball/combodone.wav");
	topbouncefx = App->audio->LoadFx("pinball/topbouncefx.wav");
	tpfx = App->audio->LoadFx("pinball/tpfx.wav");
	bigbouncefx = App->audio->LoadFx("pinball/bigbouncefx.wav");
	table = App->textures->Load("pinball/Texturas2.png");
	score = App->fonts->Load("pinball/numbers.png","0123456789", 1);
	App->audio->PlayMusic("pinball/song.ogg");
	Mix_VolumeMusic(20);

	//Rectangle measurements

	tablerect.x = 222;
	tablerect.y = 369;
	tablerect.h = 730;
	tablerect.w = 442;

	ballrect.x = 9;
	ballrect.y = 434;
	ballrect.h = 14;
	ballrect.w = 14;

	clickerrect.x = 9;
	clickerrect.y = 565;
	clickerrect.h = 56;
	clickerrect.w = 54;

	clickerrectmedium.x = 91;
	clickerrectmedium.y = 578;
	clickerrectmedium.h = 33;
	clickerrectmedium.w = 35;

	clickerrectlittle.x = 163;
	clickerrectlittle.y = 583;
	clickerrectlittle.h = 23;
	clickerrectlittle.w = 23;

	sensorlittlerect.x = 68;
	sensorlittlerect.y = 630;
	sensorlittlerect.h = 29;
	sensorlittlerect.w = 29;

	sensorlittleor.x = 22;
	sensorlittleor.y = 630;
	sensorlittleor.h = 29;
	sensorlittleor.w = 28;

	brightclicker.x = 9;
	brightclicker.y = 833;
	brightclicker.h = 54;
	brightclicker.w = 56;

	brightclickermedium.x = 84;
	brightclickermedium.y = 847;
	brightclickermedium.h = 32;
	brightclickermedium.w = 34;

	brightclickerlittle.x = 146;
	brightclickerlittle.y = 852;
	brightclickerlittle.h = 22;
	brightclickerlittle.w = 23;

	bluecircle.x = 9;
	bluecircle.y = 519;
	bluecircle.h = 37;
	bluecircle.w = 38;

	orangecircle.x = 68;
	orangecircle.y = 520;
	orangecircle.h = 35;
	orangecircle.w = 37;


	flipperLrect.x = 25;
	flipperLrect.y = 478;
	flipperLrect.h = 28;
	flipperLrect.w = 72;

	flipperRrect.x = 110;
	flipperRrect.y = 480;
	flipperRrect.h = 28;
	flipperRrect.w = 71;

	int right_bounce[10] = {
		547, 1048,
		587, 974,
		588, 957,
		581, 946,
		524, 1048
	};

	int left_bounce[14] = {
		365, 1046,
		310, 946,
		303, 957,
		303, 975,
		349, 1055,
		357, 1056,
		363, 1052
	};

	int top_bounce[12] = {
		566, 872,
		599, 870,
		610, 862,
		619, 836,
		614, 832,
		562, 868
	};

	//Initializing all the positions of the bodies

	ballpos.x = 415;
	ballpos.y = 600;
	circlepos.x = 336;
	circlepos.y = 352;
	bouncerpos.x = 145;
	bouncerpos.y = 350;
	bounpos.x = -SCREEN_WIDTH / 2.0f;
	bounpos.y = -SCREEN_HEIGHT / 2.08f;
	holepos.x = 80;
	holepos.y = 175;
	holepos2.x = 335;
	holepos2.y = 245;
	circlepos2.x = 233;
	circlepos2.y = 439;
	circlepos3.x = 258;
	circlepos3.y = 169;
	circlepos4.x = 81;
	circlepos4.y = 244;
	bouncermpos.x = 220;
	bouncermpos.y = 236;
	bouncermpos2.x = 332;
	bouncermpos2.y = 167;
	bouncermpos3.x = 204;
	bouncermpos3.y = 531;
	bouncerlpos.x = 70;
	bouncerlpos.y = 529;
	
	//Creating the bodies

	ball = App->physics->CreateCircle(ballpos.x, ballpos.y, 7);

	//BOUNCERS

	bouncer = App->physics->CreateCircleStatic(bouncerpos.x, bouncerpos.y, 27);
	bouncer->body->GetFixtureList()->SetRestitution(1.5f);

	bouncermedium = App->physics->CreateCircleStatic(bouncermpos.x, bouncermpos.y, 17);
	bouncermedium->body->GetFixtureList()->SetRestitution(1.5f);

	bouncermedium2 = App->physics->CreateCircleStatic(bouncermpos2.x, bouncermpos2.y, 17);
	bouncermedium2->body->GetFixtureList()->SetRestitution(1.5f);

	bouncermedium3 = App->physics->CreateCircleStatic(bouncermpos3.x, bouncermpos3.y, 17);
	bouncermedium3->body->GetFixtureList()->SetRestitution(1.5f);

	bouncerlittle = App->physics->CreateCircleStatic(bouncerlpos.x, bouncerlpos.y, 12);	
	bouncerlittle->body->GetFixtureList()->SetRestitution(1.5f);

	//SENSORS

	sensor = App->physics->CreateCircleSensor(circlepos.x, circlepos.y, 18);
	sensor2 = App->physics->CreateCircleSensor(circlepos3.x, circlepos3.y, 18);
	sensor3 = App->physics->CreateCircleSensor(circlepos4.x, circlepos4.y, 18);
	sensorhole = App->physics->CreateCircleSensor(holepos.x, holepos.y, 10);
	sensorhole2 = App->physics->CreateCircleSensor(holepos2.x, holepos2.y, 10);
	sensorlittle = App->physics->CreateCircleSensor(circlepos2.x, circlepos2.y, 15);

	sensorfx = App->physics->CreateRectangleSensor(410, 42, 24, 24);
	sensorvolume = App->physics->CreateRectangleSensor(39, 42, 24, 24);

	//BOUNCES

	rightBounce = App->physics->CreateChain(bounpos.x, bounpos.y, right_bounce, 10);
	leftBounce = App->physics->CreateChain(bounpos.x, bounpos.y, left_bounce, 14);
	topBounce = App->physics->CreateChain(bounpos.x, bounpos.y, top_bounce, 12);

	rightBounce->body->GetFixtureList()->SetRestitution(0.5f);
	leftBounce->body->GetFixtureList()->SetRestitution(0.5f);
	topBounce->body->GetFixtureList()->SetRestitution(0.5f);
	return ret;
}

// Load assets
bool ModuleInitScene::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(table);
	App->fonts->UnLoad(score);
	return true;
}

// Update: draw background
update_status ModuleInitScene::Update()
{
	// Table draw function

	App->renderer->Blit(table, 0, 0, &tablerect);

	// Combos logic (it restarts the state of the bouncers and blue sensors and it gives the right quantity of points)

	if (combo == 4)
	{
		App->audio->PlayFx(combofx);
		points += 40;
		combo = 0;
		collision2 = false;
		collision3 = false;
		collision4 = false;
		collision5 = false;
	}
	if (combo2 == 5)
	{
		App->audio->PlayFx(combofx);
		points += 50;
		combo2 = 0;
		collision = false;
		collision6 = false;
		collision7 = false;
		collision8 = false;
		collision9 = false;
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();

	// Flippers draw

	App->renderer->Blit(table, App->player->flipLpos.x - 72 / 2, App->player->flipLpos.y - 28 / 2, &flipperLrect, 1.0f, App->player->flipperL->GetRotation());
	App->renderer->Blit(table, App->player->flipRpos.x - 72 / 2, App->player->flipRpos.y - 28 / 2, &flipperRrect, 1.0f, App->player->flipperR->GetRotation());


	//Lifes condition of draw 
	if (lives <= 5)
	{
		ball->GetPosition(ballpos.x, ballpos.y);
		ball->listener = this;
		App->renderer->Blit(table, ballpos.x, ballpos.y, &ballrect);
	}
	
	//Collisions 
	
	sensor->GetPosition(circlepos.x, circlepos.y);
	if (!collision2)
	{
		App->renderer->Blit(table, circlepos.x, circlepos.y, &bluecircle);
	}
	else if (collision2)
	{
		App->renderer->Blit(table, circlepos.x, circlepos.y, &orangecircle);
	}

	sensor2->GetPosition(circlepos3.x, circlepos3.y);
	if (!collision4)
	{
		App->renderer->Blit(table, circlepos3.x, circlepos3.y, &bluecircle);
	}
	else if (collision4)
	{
		App->renderer->Blit(table, circlepos3.x, circlepos3.y, &orangecircle);
	}

	sensor3->GetPosition(circlepos4.x, circlepos4.y);
	if (!collision5)
	{
		App->renderer->Blit(table, circlepos4.x, circlepos4.y, &bluecircle);
	}
	else if (collision5)
	{
		App->renderer->Blit(table, circlepos4.x, circlepos4.y, &orangecircle);
	}

	sensorlittle->GetPosition(circlepos2.x, circlepos2.y);
	if (!collision3)
	{
		App->renderer->Blit(table, circlepos2.x, circlepos2.y, &sensorlittlerect);
	}
	else if (collision3)
	{
		App->renderer->Blit(table, circlepos2.x, circlepos2.y, &sensorlittleor);
	}

	bouncer->GetPosition(bouncerpos.x, bouncerpos.y);
	if (!collision)
	{
		App->renderer->Blit(table, bouncerpos.x, bouncerpos.y, &clickerrect);
	}
	else if (collision)
	{
		App->renderer->Blit(table, bouncerpos.x, bouncerpos.y, &brightclicker);
	}
	
	bouncermedium->GetPosition(bouncermpos.x, bouncermpos.y);
	if (!collision6)
	{
		App->renderer->Blit(table, bouncermpos.x, bouncermpos.y, &clickerrectmedium);
	}
	else if (collision6)
	{
		App->renderer->Blit(table, bouncermpos.x, bouncermpos.y, &brightclickermedium);
	}

	bouncermedium2->GetPosition(bouncermpos2.x, bouncermpos2.y);
	if (!collision7)
	{
		App->renderer->Blit(table, bouncermpos2.x, bouncermpos2.y, &clickerrectmedium);
	}
	else if (collision7)
	{
		App->renderer->Blit(table, bouncermpos2.x, bouncermpos2.y, &brightclickermedium);
	}

	bouncermedium3->GetPosition(bouncermpos3.x, bouncermpos3.y);
	if (!collision8)
	{
		App->renderer->Blit(table, bouncermpos3.x, bouncermpos3.y, &clickerrectmedium);
	}
	else if (collision8)
	{
		App->renderer->Blit(table, bouncermpos3.x, bouncermpos3.y, &brightclickermedium);
	}

	bouncerlittle->GetPosition(bouncerlpos.x, bouncerlpos.y);
	if (!collision9)
	{
		App->renderer->Blit(table, bouncerlpos.x, bouncerlpos.y, &clickerrectlittle);
	}
	else if (collision9)
	{
		App->renderer->Blit(table, bouncerlpos.x, bouncerlpos.y, &brightclickerlittle);
	}
	

	//Audio volume management

	if (App->input->GetMouseX() <= 44 && App->input->GetMouseY() <= 42 && App->input->GetMouseX() >= 20 && App->input->GetMouseY() >= 18 && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		mute = !mute;
	}

	if (mute)
	{
		Mix_VolumeMusic(0);
	}
	if (!mute)
	{
		Mix_VolumeMusic(20);
	}

	//if (App->input->GetMouseX() <= 410 && App->input->GetMouseY() <= 42 && App->input->GetMouseX() >= 386 && App->input->GetMouseY() >= 18 && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	mutefx = !mutefx;
	//}

	//if (mutefx)
	//{
	//	Mix_Volume
	//}
	//if (!mutefx)
	//{
	//	Mix_VolumeChunk(20);
	//}

	// Player 's draw of the Points, Highscore and Lifes 
	
	sprintf_s(text, 10, "%7d", points);
	App->fonts->BlitText(135, 35, score, text);

	sprintf_s(text2, 10, "%7d", lives);
	App->fonts->BlitText(300,35, score, text2);

	sprintf_s(text3, 10, "%7d", highscore);
	App->fonts->BlitText(260, 35, score, text3);

	sprintf_s(text4, 10, "%7d", previousscore);
	App->fonts->BlitText(200, 35, score, text4);

	return UPDATE_CONTINUE;
}

update_status ModuleInitScene::PostUpdate()
{


	// Create a test ball to do the necessary checks

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && !jointed)
	{
		App->physics->world->DestroyBody(ball->body);
		ball = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 7);
	}

	// Logic of the lifes or chances of the player

	if (ballpos.y >= 768 && lives < 5 && !jointed)
	{
		App->physics->world->DestroyBody(ball->body);
		ballpos.x = 422;
		ballpos.y = 600;
		ball = App->physics->CreateCircle(ballpos.x, ballpos.y, 7);
		lives++;
		LOG("%i", lives);
		finalscore = points;

	}



	//Restart and game over logics

	if (lives == 5 && !jointed)

	{
		if (points > highscore)
		{
			highscore = points;
		}
		App->physics->world->DestroyBody(ball->body);
		ballpos.x = 422;
		ballpos.y = 600;
		ball = App->physics->CreateCircle(ballpos.x, ballpos.y, 7);
		lives = 0;
		points = 0;
		App->endScene->Enable();
		previousscore = finalscore;
	}

	// Holes logic ( when the player enters in the hole the game destroys the ball and it create a new one in the other end of the hole)

	if (tp && !jointed)
	{
		App->audio->PlayFx(tpfx);
		App->physics->world->DestroyBody(ball->body);
		ballpos.x = 80;
		ballpos.y = 480;
		ball = App->physics->CreateCircle(ballpos.x, ballpos.y, 7);
		tp = false;
	}
	if (tp2 && !jointed)
	{
		App->audio->PlayFx(tpfx);
		App->physics->world->DestroyBody(ball->body);
		ballpos.x = 337;
		ballpos.y = 562;
		ball = App->physics->CreateCircle(ballpos.x, ballpos.y, 7);
		tp2 = false;
	}

	return UPDATE_CONTINUE;
}

void ModuleInitScene::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != NULL && bodyB != NULL) 
	{	
		if (bodyA == ball && bodyB == bouncer || bodyA == bouncer && bodyB == ball)
		{
			collision = !collision;
			App->audio->PlayFx(bonus_fx);
			if (collision == true)
			{
				combo2++;
			}
			else if (collision == false)
			{
				combo2--;
			}
			points += 5;
		}

		if (bodyA == ball && bodyB == sensor || bodyA == sensor && bodyB == ball)
		{
			collision2 = !collision2;
			App->audio->PlayFx(bluefx);
			if (collision2 == true)
			{
				combo++;
			}
			else if (collision2 == false)
			{
				combo--;
			}
			points += 10;
		}

		if (bodyA == ball && bodyB == sensorhole || bodyA == sensorhole && bodyB == ball)
		{
			tp = true;
		}
		if (bodyA == ball && bodyB == sensorhole2 || bodyA == sensorhole2 && bodyB == ball)
		{
			tp2 = true;
		}
		if (bodyA == ball && bodyB == sensorlittle || bodyA == sensorlittle && bodyB == ball)
		{
			collision3 = !collision3;
			App->audio->PlayFx(bluefx);
			if (collision3 == true)
			{
				combo++;
			}
			else if (collision3 == false)
			{
				combo--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == sensor2 || bodyA == sensor2 && bodyB == ball)
		{
			collision4 = !collision4;
			App->audio->PlayFx(bluefx);
			if (collision4 == true)
			{
				combo++;
			}
			else if (collision4 == false)
			{
				combo--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == sensor3 || bodyA == sensor3 && bodyB == ball)
		{
			collision5 = !collision5;
			App->audio->PlayFx(bluefx);
			if (collision5 == true)
			{
				combo++;
			}
			else if (collision5 == false)
			{
				combo--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == bouncermedium || bodyA == bouncermedium && bodyB == ball)
		{
			collision6 = !collision6;
			App->audio->PlayFx(bonus_fx);
			if (collision6 == true)
			{
				combo2++;
			}
			else if (collision6 == false)
			{
				combo2--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == bouncermedium2 || bodyA == bouncermedium2 && bodyB == ball)
		{
			collision7 = !collision7;
			App->audio->PlayFx(bonus_fx);
			if (collision7 == true)
			{
				combo2++;
			}
			else if (collision7 == false)
			{
				combo2--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == bouncermedium3 || bodyA == bouncermedium3 && bodyB == ball)
		{
			collision8 = !collision8;
			App->audio->PlayFx(bonus_fx);
			if (collision8 == true)
			{
				combo2++;
			}
			else if (collision8 == false)
			{
				combo2--;
			}
			points += 10;
		}
		if (bodyA == ball && bodyB == bouncerlittle || bodyA == bouncerlittle && bodyB == ball)
		{
			collision9 = !collision9;
			App->audio->PlayFx(bonus_fx);
			
			if (collision9 == true)
			{
				combo2++;
			}
			else if (collision9 == false)
			{
				combo2--;
			}
			points += 15;
		}
		if (bodyA == ball && bodyB == topBounce || bodyA == topBounce && bodyB == ball)
		{
			App->audio->PlayFx(topbouncefx);
		}
		if (bodyA == ball && bodyB == leftBounce || bodyA == leftBounce && bodyB == ball)
		{
			App->audio->PlayFx(bigbouncefx);
		}
		if (bodyA == ball && bodyB == rightBounce || bodyA == rightBounce && bodyB == ball)
		{
			App->audio->PlayFx(bigbouncefx);
		}
	}
	
}
