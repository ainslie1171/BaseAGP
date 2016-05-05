#include "pickupManager.h"

PickupManager::PickupManager(ParticleManager* particleManager, Player* player)
{
	m_pParticleManager = particleManager;
	m_pPlayer = player;
}

PickupManager::~PickupManager()
{
	while (!m_clusters.empty())
		delete m_clusters.back();
}

void PickupManager::Update(float deltaTime)
{
	for (Cluster* c : m_clusters)
	{
		if (!c->isDirty())
		{
			for (Pickup* p : c->m_pickups)
			{
				if (!p->isDirty())
				{
					//gravity
					//add accl
					p->doGravity(deltaTime);
					//check collision
					for (Pickup* p2 : c->m_pickups)
					{
						if (p != p2)
						{
							if (p->checkCollision(p2))
							{
								//res collision
								p->collisionResponse(p2);
							}
						}
					}
					
					if (m_pPlayer->collidePickup(p))// checkCollisionRaySphere(p->getPosition(), (p->getPosition() + (p->getVelocity() * deltaTime))))
					{
						m_pPlayer->collectPickup(p->getType(), rand() % 20 + 5);
						p->Dirty();
					}

					//move
					p->Update(deltaTime);
				}
			}
		}
	}


	vector<Cluster*>::iterator it = m_clusters.begin();
	while (it != m_clusters.end())
	{
		if ((*it)->isDirty())
		{
			delete (*it);
			it = m_clusters.erase(it);
		}
		else
		{
			it++;
		}
		if (m_clusters.empty()) break;
	}
}

void PickupManager::addCluster(Vector3 position, UINT num)
{
	Cluster* c = new Cluster;
	
	for (UINT i = 0; i < num; i++)
	{
		c->m_pickups.push_back(new Pickup(position, 60.0f, m_pParticleManager, nullptr, 0.06f));
	}

	m_clusters.push_back(c);
}