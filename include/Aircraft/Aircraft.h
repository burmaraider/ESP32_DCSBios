#pragma once

class Aircraft
{
public:
    Aircraft(){};
    virtual ~Aircraft(){};

    virtual void Init(){};
    virtual void Update(){};

    virtual std::vector<std::string> ActionsToDo()
    {
        return std::vector<std::string>();
    };

    virtual std::vector<ActionReceived> ActionsReceived()
    {
        return std::vector<ActionReceived>();
    };
};