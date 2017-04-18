#include "Game.h"

GameScreen::GameScreen() : Scene ()
{
	// we initialize data
	Start();
}

GameScreen::~GameScreen()
{
	delete m_bgTop;
	delete m_bgBot;
	delete m_player;
	delete m_bgm;
	delete m_sfx;
	delete font;
}

void GameScreen::Start()
{
	// We will use 2 channels for sounds: 1 = BGM, 2= Sound effects so they can be played at same time. You can set as channels as you want.

	// We clear the channels
	ndspChnWaveBufClear(1);
	ndspChnWaveBufClear(2);

	// We load our font called font.ttf in data folder. Set the data folder in MakeFile
	font = sftd_load_font_mem(font_ttf, font_ttf_size);

	// We initialize our game variables
	m_offset = 0;
	m_playerX = TOP_WIDTH / 2;
	m_playerY = HEIGHT / 2; 

	// We load our images and place them into RAM so they can be painted
	m_bgTop = sfil_load_PNG_file(IMG_BG_GAME_TOP, SF2D_PLACE_RAM);
	m_bgBot = sfil_load_PNG_file(IMG_BG_GAME_BOT, SF2D_PLACE_RAM);
	m_player = sfil_load_PNG_file(IMG_MAIN_SPRITE, SF2D_PLACE_RAM);

	// We load our sounds // PATH, CHANNEL, LOOP? -> // BGM plays with loop, but sfx just one time
	m_bgm = new sound(SND_BGM_GAME, 1, true);		
	m_sfx = new sound(SND_SFX_TAP, 2, false);

	// We play our bgm
	m_bgm->play();
}

void GameScreen::Draw()
{
	
	// 3D effect. It works this way: GFX_RIGHT SHOULD BE X POSITION - OFFSET FOR S.3D OBJECTS and GFX_LEFT X POSITION, so it makes the effect for each eye
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_draw_texture(m_bgTop, 0, 0);
	sf2d_draw_texture(m_player, m_playerX, m_playerY);
	sftd_draw_text(font, 15, 5, C_WHITE, TAPTEXTSIZE, "HELLO WORLD!!");
	sf2d_end_frame();

	// If we have activated 3D in Settings
	if (STEREOSCOPIC_3D_ACTIVATED)
	{
		// We check the offset by the slider
		m_offset = CONFIG_3D_SLIDERSTATE * MULTIPLIER_3D;
		sf2d_start_frame(GFX_TOP, GFX_RIGHT);
		sf2d_draw_texture(m_bgTop, 0, 0);
		sf2d_draw_texture(m_player, m_playerX- m_offset, m_playerY);						// 3D Object
		sftd_draw_text(font, 15- m_offset, 5, C_WHITE, TAPTEXTSIZE, "HELLO WORLD!!");		// 3D Text - Cool... huh?
		sf2d_end_frame();
	}

	// Bottom screen (We just show an image)
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	sf2d_draw_texture(m_bgBot, 0, 0);
	sf2d_end_frame();
}

void GameScreen::Update()
{
	// Things that happens in your game... update the player or something
}

void GameScreen::CheckInputs()
{
	hidScanInput();

	/* Check this as it may be so much xD */
	if ((hidKeysDown() & KEY_UP))
	{
		m_playerY--;
	}

	if ((hidKeysDown() & KEY_DOWN))
	{
		m_playerY++;
	}

	if ((hidKeysDown() & KEY_RIGHT))
	{
		m_playerX++;
	}

	if ((hidKeysDown() & KEY_LEFT))
	{
		m_playerX--;
	}

	// We Exit pressing Select
	if ((hidKeysDown() & KEY_SELECT))
	{
		SceneManager::instance()->SaveDataAndExit();
	}
}

