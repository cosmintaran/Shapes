#pragma once

namespace SV::CORE {

	class ICommand {

	public:
		virtual void Execute() = 0;
		virtual void CanExecute() = 0;
	};

}
