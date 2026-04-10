#pragma once 

namespace bank
{
    enum class VulnProfile {
        Easy,
        Medium,
        Hard
    };

struct Security_config
{
    VulnProfile profile = VulnProfile::Easy;

    bool idor_enabled = true;
    bool weak_sessions = true;
    bool race_window_enabled = true;
    bool unsafe_input_parsing = true;
    bool weak_passwords = true;
    bool debug_endpoints = true;
};

Security_config make_profile(VulnProfile profile);

}