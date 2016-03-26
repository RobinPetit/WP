// WizardPoker headers
#include "client/Gui/CreatureGui.hpp"
#include "client/ClientCardData.hpp"
#include "client/Gui/SpellGui.hpp"
#include "client/Gui/CardWidget.hpp"

CardWidget::CardWidget()
{
	setSize(CardGui::getSize());
}

CardWidget::CardWidget(const CommonCardData* cardData)
{
	setCardData(cardData);
	setSize(CardGui::getSize());
}

void CardWidget::setCardData(const CommonCardData* cardData)
{
	assert(cardData->isCreature() or cardData->isSpell());
	if(cardData->isCreature())
	{
		const ClientCreatureData* castedCardData{dynamic_cast<const ClientCreatureData*>(cardData)};
		assert(castedCardData != nullptr);
		_card.reset(new CreatureGui(castedCardData->getName(),
		            castedCardData->getDescription(),
		            castedCardData->getCost(),
		            castedCardData->getAttack(),
		            castedCardData->getHealth(),
		            castedCardData->getShield(),
		            static_cast<ShieldType>(castedCardData->getShieldType())));
	}
	else
	{
		const ClientSpellData* castedCardData{dynamic_cast<const ClientSpellData*>(cardData)};
		assert(castedCardData != nullptr);
		_card.reset(new SpellGui(castedCardData->getName(),
		            castedCardData->getDescription(),
		            castedCardData->getCost()));
	}
}

void CardWidget::setPosition(const tgui::Layout2d& position)
{
	ClickableWidget::setPosition(position);
	if(_card)
		_card->setPosition(position.x.getValue(), position.y.getValue());
}

void CardWidget::setSize(const tgui::Layout2d& size)
{
	ClickableWidget::setSize(size);
	if(_card)
		_card->setScale(size.x.getValue()/_card->getSize().x, size.y.getValue()/_card->getSize().y);
}

void CardWidget::setOrigin(float x, float y)
{
	_card->setOrigin(x, y);
}

void CardWidget::rotate(float angle)
{
	_card->rotate(angle);
}

void CardWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if(_card)
		target.draw(*_card, states);
}
