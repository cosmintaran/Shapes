#pragma once
namespace SV::CORE {

	class Timestep {
	public:
		Timestep(double time = 0.0)
			:_time(time)
		{
		}

		operator double() const { return _time; }
		double GetSeconds()const { return _time; }
		double GetMiliseconds()const { return _time * 1000.0; }

	private:
		//in seconsd
		double _time;
	};

}