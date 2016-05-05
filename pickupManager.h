#ifndef _PICKUPMANAGER_H_
#define _PICKUPMANAGER_H_

#include "pickup.h"
#include "player.h"

class PickupManager
{
private:
	struct Cluster
	{
		vector<Pickup*> m_pickups;

		bool isDirty()
		{
			bool result = true;
			for (Pickup* p : m_pickups)
			{
				if (!p->isDirty())
					result = false;
			}
			return result;
		}

		~Cluster()
		{
			while (!m_pickups.empty())
			{
				delete m_pickups.back();
				m_pickups.pop_back();
			}
		}
	};

	ParticleManager* m_pParticleManager;
	Player*	m_pPlayer;
	vector<Cluster*> m_clusters;

public:
	PickupManager(ParticleManager* particleManager, Player* player);
	~PickupManager();

	void Update(float deltaTime);
	void addCluster(Vector3 position, UINT num);
	void cleanCluster(void);
};

#endif//_PICKUPMANAGER_H_

/*
Change Log:

*/