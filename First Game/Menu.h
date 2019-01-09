#pragma once
#include <GameEmGine.h>
class Menu:public Scene
{
public:
	// Set menu screen
	void menuInite()
	{
		menu = true;

		mod.push_back(new Model("Models/Screen/Menu/menu.obj"));
		game.addModel(mod.back());
		mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);

		LightSource::setSceneAmbient({255,255,255,255});
	}

	// doing the update for menu screen
	void updateMenu()
	{
		if(fadein)
		{
			splashT += 0.01f;
			splashAmbient = (GLubyte)lerp(0, 255, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient >= 250)
			{
				fadein = false;
				splashT = 0;
				splashAmbient = 255;
				LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			}
		}

		if(Xinput::buttonPressed(game.getController(0).buttons.B))
		{
			fadeout = true;
		}

		if(fadeout)
		{
			splashT += 0.01f;
			splashT = splashT > 1 ? 1 : splashT;
			splashAmbient = (GLubyte)lerp(255, 0, splashT);
			LightSource::setSceneAmbient({splashAmbient,splashAmbient,splashAmbient,splashAmbient});
			if(splashAmbient <= 5)
			{
				fadein = true;
				fadeout = false;
				splashT = 0;
				splashAmbient = 255;
				playGame = true;
				intro = false;

				game.removeModel(mod[0]);
				mod.clear();
				GamePlayInit();

			}
		}

	}

	//updates within game loop
	void update(double dt)
	{

		if(menu)
		{
			updateMenu();
		}

		if(playGame)
		{

			menu = false;
			static float  time = 0;
			time += (float)dt;
			//static float coolDown = 0;
			static float duration = 0;
			static bool f = true;
			static bool bossActive = true;

			float move = .1f;

			static float pointSize = 50.0f;
			//printf("%f\n", dt);
			static Player* player;
			static Boss*CandyMan = (Boss*)mod[8];
			//drawHealth(CandyMan->getHealth());

			static vector<float> timer[4];
			static vector<Model*> pMissiles[4];
			static vector<Coord3D> missileVelocity[4];
			static vector<Model*> bullets[4];
			static vector<Coord3D> velocity[4];
			static bool makeShitLessCancer[4], makeShitLessCancer2[4];//stops the creation of bullets when trigger is healed down
			static float  curveroni[4] = {0 ,0,0,0};
			static bool hasTarget[4] = {0 ,0,0,0};

			static bool dead[4];

			static float lastTime = (float)clock() / CLOCKS_PER_SEC;

			static Animation squash[4];

			static bool init = false;

			static float angle[4] = {180,180,180,180};



			for(int a = 0; a < 4; a++)
			{
				if(!dead[a])
				{
					curveroni[a] += .01f;
					if(curveroni[a] >= 1)
					{
						hasTarget[a] = false;
					}
					curveroni[a] = fmodf(curveroni[a], 1);

					static Coord3D bossTarget[4];

					//gets a   target for missile (player 1,2,3 or 4) randomly
					if(!hasTarget[a])
					{

						bossTarget[a] = mod[a]->getTransformer().getPosition();

						hasTarget[a] = true;
					}

					if(mod[8])
						if(hasTarget[a])
						{
							Coord3D
								p1[4],
								p2[4],
								c1[4],
								c2[4];
							Coord3D cat[4];
							Coord3D  pointPosition[4];
							p1[a] = mod[8]->getTransformer().getPosition() + Coord3D(0.0f, 8.0f, 2.0f),//start point
								p2[a] = bossTarget[a],//end point 
								c1[a] = p1[a] - Coord3D{0,100,100},//controle point
								c2[a] = p2[a] - Coord3D{0,150,100};//controle point

							cat[a] = catmull
							(
								c1[a],
								p1[a],
								p2[a],
								c2[a],
								curveroni[a]
							);
							pointPosition[a] = cat[a];
							mod[44 + a]->getTransformer().setPosition(pointPosition[a].x, pointPosition[a].y, pointPosition[a].z);
						}
				}
			}

			if(!init)
			{

				for(int a = 0; a < 4; a++)
				{
					squash[a].addDir("Models/RIP/Rip Ani/");
					squash[a].setAnimationSpeed(.2f);
					//	squash[a].repeat(true);
				}

			}

			if(init)
				if(movePlayer)
					for(int a = 0; a < 4; a++)
					{
						if(game.isControllerConnected(a))
						{
							if(!dead[a])
							{
								player = (Player*)mod[a];
								Xinput p1 = game.getController(a);

								if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
								{

									angle[a] = acosf(p1.Coord2D_sticks[RS].x /
										sqrtf(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
											+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
									angle[a] += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle[a]) * 2 : 0) + 90;//90 represents the start angle
									angle[a] = fmodf(angle[a], 360);
								}

								/// - Missile Collisions with Player - ///
								for(int b = 0; b < 4; b++)
								{
									bool collision = collisions3D(player, mod[60 + b]);
									if(collision)
									{
										curveroni[a] = 1;
										mod[44 + b]->getTransformer().setPosition(mod[8]->getCenter());
										player->setHealth(player->getHealth() - 35);
										//Coord3D test = ;

									}
								}
								//Player comes near Boss
								if(collisionsB(player, CandyMan))
								{
									player->getTransformer().setPosition(player->getTransformer().getPosition().x, player->getTransformer().getPosition().y, player->getTransformer().getPosition().z - 15);
									player->setHealth(player->getHealth() - 35);
								}
								if(player->getHealth() <= 0)
								{
									dead[a] = true;
									mod[22 + a]->setColour(player->getColour());
									mod[22 + a]->getTransformer().setScale(0.75f * 2, 1 * 2, 0.5 * 2), mod[22 + a]->getTransformer().setPosition(player->getTransformer().getPosition()), mod[22 + a]->getTransformer().setRotation({0.0f,270.0f,0.0f});
									game.addModel(mod[22 + a]);
									mod[22 + a]->addAnimation("squash", &squash[a]);

									mod[22 + a]->setAnimation("squash");
									game.removeModel(player);
								}
								if(p1.triggers[RT] >= .95 && !makeShitLessCancer[a])
								{
									if(player->getBulletCount() > 0)
									{
										makeShitLessCancer[a] = true;

										bullets[a].push_back(nullptr);
										game.addModel(bullets[a].back() = new Model(*mod[48]));
										bullets[a].back()->getTransformer().reset();
										bullets[a].back()->setColour(player->getColour());
										Coord3D pos = mod[a]->getTransformer().getPosition();
										bullets[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
										bullets[a].back()->getTransformer().setScale(0.13f);

										bullets[a].back()->getTransformer().setRotation({90 , angle[a] ,0});


										float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
										float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

										velocity[a].push_back(Coord3D());
										velocity[a].back() = Coord3D(cosVal * move * 3, 0, sinVal * move * 3);

										timer[a].push_back(0);
										audio.createAudioStream("pew.wav");
										audio.play();
										player->setBulletCount(player->getBulletCount() - 1);
									}
								}
								else if(p1.triggers[RT] < .95 && makeShitLessCancer[a])
									makeShitLessCancer[a] = false;

								/// - Button Presses on controller - ///
								//Start button quits game
								if(p1.buttonPressed(p1.buttons.START))
								{
									puts("\nExiting Game\n");
									game.exit();
								}
								if(p1.buttonPressed(p1.buttons.X))
								{
									player->setBulletCount(30);
									puts("RELOADING!!!\n");
								}
								if(p1.buttonPressed(p1.buttons.Y))
								{
									if(time - player->getTimeSinceLastMissile() >= 3)
									{
										pMissiles[a].push_back(nullptr);
										game.addModel(pMissiles[a].back() = new Model(*mod[44]));
										pMissiles[a].back()->getTransformer().reset();
										pMissiles[a].back()->setColour(player->getColour());
										Coord3D pos = mod[a]->getTransformer().getPosition();
										pMissiles[a].back()->getTransformer().setPosition(pos.x, pos.y + .1f, pos.z);
										pMissiles[a].back()->getTransformer().setScale(0.4f);

										pMissiles[a].back()->getTransformer().setRotation({0 , angle[a] ,0});

										float cosVal = cos((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));
										float sinVal = sin((float)(fmodf(angle[a] - 90, 360)*(M_PI / 180)));

										missileVelocity[a].push_back(Coord3D());
										missileVelocity[a].back() = Coord3D(cosVal * move * 6, 0, sinVal * move * 6);
										player->setTimeSinceLastMissile(time);

										timer[a].push_back(0);
										//audio.createStream("pew.wav");
										//audio.play();
										puts("SPECIAL ABILITY\n");
									}
								}

								/// - Left Trigger to Dash - ///

								if(p1.triggers[LT] >= .95)
								{
									static float coolDown[4];

									//get deltaTime put into duraction variable

									if(time - coolDown[a] >= 3)
									{
										if(f == true)
										{
											duration = time;
											f = false;
										}
										move = 0.5f;
										if(time - 0.1f >= duration)
										{
											move = 0.1f;
											//If triggers up then coolDown = time;
											coolDown[a] = time;
											f = true;
										}
									}

								}
								else//Do the same with the LT button, have it so will only work every X seconds.
								{
									move -= .001f;
									if(move <= .1f)
										move = .1f;
									//f = false;
									makeShitLessCancer2[a] = false;
								}

								mod[a]->getTransformer().setRotation({0,angle[a], 0});
								mod[a]->getTransformer().translateBy(p1.Coord2D_sticks[LS].x * move, 0, p1.Coord2D_sticks[LS].y * move); //move player
								float speed = p1.Coord2D_sticks[LS].x*p1.Coord2D_sticks[LS].x + p1.Coord2D_sticks[LS].y*p1.Coord2D_sticks[LS].y;

								mod[0]->getTransformer().setScale(0.85f, 1.5f, 1.0f);

								if(!collisions(mod[a], mod[59]))
									mod[a]->getTransformer().setPosition(abs(mod[a]->getTransformer().getPosition().x) > mod[59]->getWidth() / 2 ? mod[a]->getTransformer().getPosition().x < 0 ? -mod[59]->getWidth() / 2 : mod[59]->getWidth() / 2 : mod[a]->getTransformer().getPosition().x,
										0,
										abs(mod[a]->getTransformer().getPosition().z) > mod[59]->getDepth() / 2 ? mod[a]->getTransformer().getPosition().z < 0 ? -mod[59]->getDepth() / 2 : mod[59]->getDepth() / 2 : mod[a]->getTransformer().getPosition().z);

								if(!speed)
									mod[a]->getAnimation("walk")->pause();
								else
								{
									mod[a]->getAnimation("walk")->play();
									mod[a]->setAnimation("walk");
									mod[a]->getAnimation("walk")->setAnimationSpeed(.25f / speed);
								}
							}
							/// - Bullet Collisions - ///
							for(unsigned b = 0; b < bullets[a].size(); b++)
								if(bullets[a][b])
								{
									timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
									bullets[a][b]->getTransformer().translateBy(velocity[a][b].x, velocity[a][b].y, velocity[a][b].z);

									if(timer[a][b] >= 1)
									{
										game.removeModel(bullets[a][b]);
										bullets[a].erase(bullets[a].begin() + b);
										velocity[a].erase(velocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										break;
									}

									if(mod[8])
										if(collisions(bullets[a][b], mod[8]))
										{
											game.removeModel(bullets[a][b]);
											bullets[a].erase(bullets[a].begin() + b);
											velocity[a].erase(velocity[a].begin() + b);
											timer[a].erase(timer[a].begin() + b);
											//Boss a.k.a model 8, is now called CandyMan for teh purposes of functions.
											CandyMan->setHealth(CandyMan->getHealth() - 10);// When hit takes damage
											if(CandyMan->getHealth() <= 0)
											{
												game.removeModel(CandyMan); // If health = 0 then boss dead
												//	mod[8] = nullptr;
												bossActive = false;
												puts("Killed The BOSS\n");
											}
											puts("Hit The BOSS\n");
											break;
										}
								}
							for(unsigned b = 0; b < pMissiles[a].size(); b++)
								if(pMissiles[a][b])
								{
									timer[a][b] += (float)clock() / CLOCKS_PER_SEC - lastTime;
									pMissiles[a][b]->getTransformer().translateBy(missileVelocity[a][b].x, missileVelocity[a][b].y, missileVelocity[a][b].z);

									if(timer[a][b] >= 1)
									{
										game.removeModel(pMissiles[a][b]);
										pMissiles[a].erase(pMissiles[a].begin() + b);
										missileVelocity[a].erase(missileVelocity[a].begin() + b);
										timer[a].erase(timer[a].begin() + b);
										break;
									}

									if(mod[8])
										if(collisions(pMissiles[a][b], mod[8]))
										{
											game.removeModel(pMissiles[a][b]);
											pMissiles[a].erase(pMissiles[a].begin() + b);
											missileVelocity[a].erase(missileVelocity[a].begin() + b);
											timer[a].erase(timer[a].begin() + b);
											//Boss a.k.a model 8, is now called CandyMan for teh purposes of functions.
											CandyMan->setHealth(CandyMan->getHealth() - 50);// When hit takes damage
											if(CandyMan->getHealth() <= 0)
											{
												game.removeModel(CandyMan); // If health = 0 then boss dead
												//	mod[8] = nullptr;
												bossActive = false;
												puts("Killed The BOSS\n");
											}
											puts("Hit The BOSS\n");
											break;
										}
								}


						}
					}
				else
				{
					//	mod[a]->getAnimation("squash")->update(mod[a]->getShader(),mod[a]->getMesh());
				}

			lastTime = (float)clock() / CLOCKS_PER_SEC;

			if(!movePlayer)
				if(game.isControllerConnected(0))
				{
					Xinput p1 = game.getController(0);

					p1.numButtons;
					p1.numSticks;
					float angle1 = 0;
					if(p1.Coord2D_sticks[RS].x || p1.Coord2D_sticks[RS].y)
					{

						angle1 = acosf(p1.Coord2D_sticks[RS].x /
							sqrt(p1.Coord2D_sticks[RS].x*p1.Coord2D_sticks[RS].x
								+ p1.Coord2D_sticks[RS].y*p1.Coord2D_sticks[RS].y)) * (180 / (float)M_PI);
						angle1 += (p1.Coord2D_sticks[RS].y < 0 ? (180 - angle1) * 2 : 0) + 90;//90 represents the start angle
						angle1 = fmodf(angle1, 360);
					}

					if(Xinput::buttonPressed(p1.buttons.A))
						printf("%d\n", p1.buttons.A);

					//move camera
					move *= 2;

					game.moveCameraPositionBy({p1.Coord2D_sticks[LS].x * move , 0 * move, p1.Coord2D_sticks[LS].y * move});//move camera
					game.moveCameraAngleBy(ang * (abs(p1.Coord2D_sticks[RS].x) + abs(p1.Coord2D_sticks[RS].y)), {p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0});//rotate camera
					//game.getMainCamera()->getTransformer().rotateBy({ ang *p1.Coord2D_sticks[RS].y ,ang *p1.Coord2D_sticks[RS].x ,0}, { p1.Coord2D_sticks[RS].y  ,p1.Coord2D_sticks[RS].x, 0 });
					game.moveCameraPositionBy({0 ,p1.triggers[LT] * -move,0});//move out
					game.moveCameraPositionBy({0 ,p1.triggers[RT] * move,0});//move out
					move /= 2;
				}
			//}
			init = true;
		}
	}
};


