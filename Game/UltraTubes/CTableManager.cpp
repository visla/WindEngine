/*
	Author  : Vuk Vranjkovic
	File    : CTableManager.cpp
	Date    : 28.03.2006.
	Version : 1.00
*/
#include <CTableManager.h>
#include <CProfile.h>
#include <stdio.h>

CTableManager::CTableManager()
{
	char buf[3];
	for(unsigned i=0; i!=MAX_PLAYERS; i++)
	{
		profiles[i]=0;
		teams[i] = std::string("team") + itoa(i+1,buf,10);
	}
}


void CTableManager::sort(void)
{
	char buf[3];
	unsigned to_put = 0;
	for(unsigned i=0; i!=MAX_PLAYERS; i++)
	{
		std::string to_cmp = std::string("team") + itoa(i+1,buf,10);
		for(unsigned j=0; j!=MAX_PLAYERS; j++)
		{
			if(teams[j]==to_cmp)
			{
				if(to_put!=j)
				{
					swap(j,to_put);
				}
				to_put++;
			}
		}
	}
}

void CTableManager::reset()
{
	char buf[3];
	for(unsigned i=0; i!=MAX_PLAYERS; i++)
	{
		profiles[i]=0;
		teams[i] = std::string("team") + itoa(i+1,buf,10);
	}
}

void CTableManager::swap(unsigned char p1, unsigned char p2)
{
	std::string buf = teams[p1];
	CProfile *tmp = profiles[p1];

	teams[p1] = teams[p2];
	profiles[p1] = profiles[p2];

	teams[p2] = buf;
	profiles[p2] = tmp;
}

inline std::string _create_team_str(unsigned num){char buf[3]; return (std::string("team")+itoa(num,buf,10));}
void CTableManager::setProfileAt(CProfile *p, unsigned char pos)
{
	if(pos>=MAX_PLAYERS) return;

	profiles[pos] = p;
	teams[pos] = _create_team_str(p->TeamNumber);
}