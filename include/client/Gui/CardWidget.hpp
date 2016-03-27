#ifndef _CARD_WIDGET_CLIENT_HPP
#define _CARD_WIDGET_CLIENT_HPP

// External headers
#include <TGUI/Widgets/ClickableWidget.hpp>
// WizardPoker header
#include "client/Gui/CardGui.hpp"
#include "common/CardData.hpp"
// std-C++ headers
#include <vector>

/// Wrapper around a CardGui to be able to add them easily in TGUI containers.
/// The implementation is inspired from the one of tgui::Canvas.
class CardWidget : public tgui::ClickableWidget
{
	public:
		typedef std::shared_ptr<CardWidget> Ptr;
		typedef std::shared_ptr<const CardWidget> ConstPtr;

		typedef std::vector<Ptr> DisplayableCardsCollection;

		CardWidget();

		CardWidget(const CommonCardData* cardData);

		void setCardData(const CommonCardData* cardData);

		//static CardWidget::Ptr copy(CardWidget::ConstPtr other);

		virtual void setPosition(const tgui::Layout2d& position) override;
		using tgui::ClickableWidget::setPosition;

		virtual void setSize(const tgui::Layout2d& size) override;
		using tgui::ClickableWidget::setSize;

		void setOrigin(float x, float y);

		void rotate(float angle);

		CardGui *getCard();

		//virtual tgui::Widget::Ptr clone() const override;

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		std::unique_ptr<CardGui> _card;
};

#endif  // _CARD_WIDGET_CLIENT_HPP
