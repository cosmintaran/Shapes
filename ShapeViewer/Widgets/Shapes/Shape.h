#pragma once
#include "Widgets/Envelope.h"

namespace SV::Shapes {

	class Shape {
	public:
		virtual ~Shape() = default;
		[[nodiscard]] virtual size_t Count() const = 0;
		[[nodiscard]] virtual const Envelope& GetEnvelope()const = 0;
		virtual void SetSelected(bool value) = 0;
		[[nodiscard]] bool IsSelected() const { return _isSelected; }

	protected:
		bool _isSelected = false;
	};
}
