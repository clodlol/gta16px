#include <unordered_map>

class InputManager
{
public:
    bool IsActionActive(const std::string &action);
    void Update();

private:
    std::unordered_map<std::string, bool> actionMap{};
    // Returns whether an action is active or not
};