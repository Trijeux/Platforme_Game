#include "World.h"
#include <iostream>
#include <chrono>

const  int World::point_treshold_ = 1000;

void World::LoadTextures()
{
	try
	{
		textures_.load(texture::kPlayerDie, "Textures/die_sheet.png");
		textures_.load(texture::kPlayerIdle, "Textures/idle_sheet.png");
		textures_.load(texture::kPlayerMove, "Textures/run_sheet.png");
		textures_.load(texture::kPlayerJump, "Textures/jump_sheet.png");
		textures_.load(texture::kPlayerFall, "Textures/fall_sheet.png");
		textures_.load(texture::kPlayerCrouch, "Textures/crouch_sheet.png");
		textures_.load(texture::kPlatform, "Textures/platform.png");
		textures_.load(texture::kSpikes, "Textures/spikes.png");
		textures_.load(texture::kSpawnPoint, "Textures/spawn_sheet.png");
		textures_.load(texture::kBat, "Textures/bat_sheet.png");
		textures_.load(texture::kBat2, "Textures/bat2_sheet.png");
		textures_.load(texture::kCoin, "Textures/coin_sheet.png");
		textures_.load(texture::kPortalIdle, "Textures/portal_idle.png");
		textures_.load(texture::kPortalMove, "Textures/portal_move.png");
		textures_.load(texture::kBackground, "Textures/background.png");
	}
	catch (...)
	{
		throw;
	}

	return;
}




void World::MultithreadEnemyCollisions(std::vector<Bat*> enemies, int firstIndex, int n)
{
	for (auto i = enemies.begin() + firstIndex; i < enemies.begin() + firstIndex + n; i++)
	{
		(*i)->ResolveCollision(*m_player_);
		for (auto j = m_blocks_.begin(); j != m_blocks_.end(); j++)
		{
			(*i)->ResolveCollision(**j);
		}
	}
}


void World::HandleBetterEnemyCollisions()
{
	int bats_per_thread = m_enemies_.size()/4;
	std::thread t4(&World::MultithreadEnemyCollisions, this, m_enemies_,3*bats_per_thread,bats_per_thread+m_enemies_.size()%4);
	std::thread t1(&World::MultithreadEnemyCollisions, this, m_enemies_, 0, bats_per_thread);
	std::thread t2(&World::MultithreadEnemyCollisions, this, m_enemies_, bats_per_thread, bats_per_thread);
	std::thread t3(&World::MultithreadEnemyCollisions, this, m_enemies_, 2*bats_per_thread,bats_per_thread);
	t4.join();
	t1.join();
	t2.join();
	t3.join();
}


void World::UpdateEnemies(sf::Time dt)
{
	for (auto i = m_enemies_.begin(); i != m_enemies_.end(); i++)
		(*i)->update(dt);
}

void World::HandlePlayerCollisions()
{
	if (m_player_->HasMoved())
	{
		for (auto i = m_blocks_.begin(); i != m_blocks_.end(); i++)
			m_player_->ResolveCollision(**i);
		for (auto i = m_spawn_points_.begin(); i != m_spawn_points_.end(); i++)
			m_player_->ResolveCollision(**i);
		for (auto i = m_coins_.begin(); i != m_coins_.end(); i++)
			m_player_->ResolveCollision(**i);
		m_player_->ResolveCollision(*m_portal_);
	}
}


void World::UpdateAnimations(sf::Time dt)
{
	UpdateBackground();
	if(m_player_!=nullptr)
		m_player_->UpdateAnimation(dt);
	if (m_portal_ != nullptr&&m_portal_->IsOpen())
		m_portal_->UpdateAnimation(dt);
	for (auto i = m_spawn_points_.begin(); i != m_spawn_points_.end(); i++)
		(*i)->UpdateAnimation(dt);
	for (auto i = m_enemies_.begin(); i != m_enemies_.end(); i++)
		(*i)->UpdateAnimation(dt);
	for (auto i = m_coins_.begin(); i != m_coins_.end(); i++)
		(*i)->UpdateAnimation(dt);
}

void World::CleanCoins()
{
	auto i = m_coins_.begin();
	while (i != m_coins_.end())
	{
		if ((*i)->IsDestroyed())
		{
			i = m_coins_.erase(i);
		}
		else
			i++;
	}
}

void World::draw(sf::RenderTarget & target, sf::RenderStates state) const
{
	target.draw(background, state);
	for (auto i = m_spawn_points_.begin(); i != m_spawn_points_.end(); i++)
		if((*i)->OnScreen(current_view_))
			target.draw(**i,state);
	if (m_portal_ != nullptr && m_portal_->IsOpen())
		target.draw(*m_portal_, state);
	if (m_player_ != nullptr)
		target.draw(*m_player_, state);
	for (auto i = m_blocks_.begin(); i != m_blocks_.end(); i++)
		if ((*i)->OnScreen(current_view_))
			target.draw(**i,state);
	for (auto i = m_coins_.begin(); i != m_coins_.end(); i++)
		if ((*i)->OnScreen(current_view_))
			target.draw(**i, state);
	for (auto i = m_enemies_.begin(); i != m_enemies_.end(); i++)
		if ((*i)->OnScreen(current_view_))
			target.draw(**i, state);
	

}

void World::UpdateBackground(/*sf::Vector2f _position, sf::Vector2f _size*/)
{
	background.setPosition(sf::Vector2f{ -6000,-5000 });
	background.setTexture(textures_.get(texture::Id::kBackground));
	background.setTextureRect(sf::IntRect(sf::Vector2i{ 0, 0 }, sf::Vector2i{12000,12000}));

}


World::World()
{	
}


World::~World()
{
	delete m_player_;
	delete m_portal_;
	for (auto i = m_spawn_points_.begin(); i != m_spawn_points_.end(); i++)
		delete (*i);
	for (auto i = m_blocks_.begin(); i != m_blocks_.end(); i++)
		delete (*i);
	for (auto i = m_enemies_.begin(); i != m_enemies_.end(); i++)
		delete (*i);
	for (auto i = m_coins_.begin(); i != m_coins_.end(); i++)
		delete (*i);

}

bool World::update(sf::Time dt, bool key_pressed[6])
{
	if (m_player_ != nullptr)
	{

		
		m_player_->update(dt, key_pressed);
		HandlePlayerCollisions();
		if (m_player_->GetScore()>=point_treshold_)
			m_portal_->OpenPortal();
		UpdateEnemies(dt);
		HandleBetterEnemyCollisions();
		UpdateAnimations(dt);
		
		CleanCoins();
		
		if (!m_player_->respawn())
			return false;
		else if (m_player_->FinishedCurrentLevel())
			return false;
		else
			return true;
		
	}
	return false;
}

sf::Vector2f World::GetPlayerPostition()
{
	return m_player_->GetPosition();
}

void World::AddObject(Player * new_player)
{
	if (m_player_ != nullptr)
		delete m_player_;
	m_player_ = new_player;
	m_player_->AddAnimation(animation::Type::kIdle, textures_.get(texture::Id::kPlayerIdle), sf::Time(sf::seconds(0.5f)), 4, true);
	m_player_->AddAnimation(animation::Type::kMove, textures_.get(texture::Id::kPlayerMove), sf::Time(sf::seconds(0.5f)), 6, true);
	m_player_->AddAnimation(animation::Type::kFall, textures_.get(texture::Id::kPlayerFall), sf::Time(sf::seconds(0.5f)), 2, true);
	m_player_->AddAnimation(animation::Type::kJump, textures_.get(texture::Id::kPlayerJump), sf::Time(sf::seconds(0.76f)), 13, false);
	m_player_->AddAnimation(animation::Type::kDeath, textures_.get(texture::Id::kPlayerDie), sf::Time(sf::seconds(0.76f)), 5, false);
	m_player_->AddAnimation(animation::Type::kCrouch, textures_.get(texture::Id::kPlayerCrouch), sf::Time(sf::seconds(0.5f)), 4, true);
}

void World::AddObject(Entity * new_block)
{
	if (new_block->GetType() == object::Type::kWall)
	{
		new_block->SetTexture(textures_.get(texture::Id::kPlatform), true);
		new_block->SetColor(sf::Color(47,57,66));
	}
	else
		new_block->SetTexture(textures_.get(texture::Id::kSpikes), false);
	m_blocks_.push_back(new_block);
}

void World::AddObject(SpawnPoint * new_spawn_point)
{

	new_spawn_point->AddAnimation(textures_.get(texture::Id::kSpawnPoint), sf::Time(sf::milliseconds(550)), 4, true);
	m_spawn_points_.push_back(new_spawn_point);
}

void World::AddObject(Bat * new_enemy)
{
	/*if (rand() % 2)
		(new_enemy)->AddAnimation(textures_.get(texture::Id::kBat), sf::Time(sf::seconds(0.5f)), 5, true);
	else*/
	(new_enemy)->AddAnimation(textures_.get(texture::Id::kBat2), sf::Time(sf::seconds(0.5f)), 5, true);
	m_enemies_.push_back(new_enemy);
}

void World::AddObject(Coin * new_coin)
{
	new_coin->AddAnimation(textures_.get(texture::Id::kCoin), sf::Time(sf::seconds(0.5f)), 5, true);
	m_coins_.push_back(new_coin);
}

void World::AddObject(Portal * new_portal)
{
	if (m_portal_ != nullptr)
		delete m_portal_;
	m_portal_ = new_portal;
	m_portal_->AddAnimation(animation::Type::kIdle, textures_.get(texture::Id::kPortalIdle), sf::Time(sf::seconds(0.75f)), 8, true);
	m_portal_->AddAnimation(animation::Type::kMove, textures_.get(texture::Id::kPortalMove), sf::Time(sf::seconds(0.75f)), 8, false);
}

int World::PlayerHealth()
{
    return m_player_->RemainingLives();
}

int World::PlayerScore()
{
	return m_player_->GetScore();
}

void World::SetCurrentView(sf::View current_view)
{
	current_view_ = current_view;
}

int World::GetPointTreshold()
{
	return point_treshold_;
}

bool World::PlayerVictory()
{
	return m_player_->FinishedCurrentLevel();
}

sf::Vector2f World::GetPlayerVelocity()
{
	return m_player_->GetVelocity();
}
