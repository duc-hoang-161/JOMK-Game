#include "Character\Monsters\Box.h"
#include "Helper\Helper.h"
#include "Character\Monsters\Monster.h"


Box::Box()
	: Monster("Characters/Box/BoxAnimation.json")
{
	m_traceDistance = 200;
	m_healthBarBackground->setVisible(false);
	m_healthBar->setVisible(false);
}

void Box::update(float deltaTime)
{
	Monster::update(deltaTime);	// super

	if (m_HP <= 0) {

		CallFunc *func = CallFunc::create([this]() {
			this->setVisible(false);

		});



	}
}



Box::~Box()
{

}