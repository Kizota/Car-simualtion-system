#ifndef STATECONTROL_HPP
#define STATECONTROL_HPP

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

    T GetState()
    {
        return state;
    }

    T GetPreState()
    {
        return preState;
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