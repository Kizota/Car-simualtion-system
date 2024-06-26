#ifndef STATECONTROL_HPP
#define STATECONTROL_HPP

//TODO - checking why method could not be define seperately in the src - undefine refference error 
template <typename T>
class StateControl
{
private:
    T state;
    T preState;

public:
    StateControl(T state, T preStata) : state(state), preState(preState)
    {
    }

    T GetState(bool takeLast)
    {
        if (takeLast)
            return preState;

        return state;
    }
    bool IsNewState()
    {
        return state != preState;
    }

    void Refresh()
    {

        preState = state;
    }

    void UpdateState(T newState)
    {
         state = newState;
    }
};

#endif