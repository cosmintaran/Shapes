#pragma once
#include "ICommand.h"


namespace SV::CORE {
	template<typename T>
	class Command
		:public ICommand {

	public:
		void Execute() override;
		void CanExecute()override;

		Command(const T& lambda)
			: _lambda(lambda){

		}

	private:
		const T& _lambda;

	};

	template<typename T>
	inline void Command<T>::Execute()
	{
		_lambda();
	}
	template<typename T>
	inline void Command<T>::CanExecute()
	{
		return true;
	}
}
