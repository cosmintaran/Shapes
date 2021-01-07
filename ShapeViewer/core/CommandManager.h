#pragma once
#include <unordered_map>
#include <memory>

namespace SV::CORE {
    class ICommand;

    class CommandManager {

    public:

        void RegisterCommand(long memAdress, ICommand* object) {

        }

        void UnRegisterCommand(long memAdress) {

        }

    private:
        std::unordered_map<long, std::unique_ptr<ICommand>> _commandMap;

    };

}
