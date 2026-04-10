#include "bank/security_config.hpp"

namespace bank
{
    SecurityConfig make_profile(VulnProfile profile)
    {
        SecurityConfig cfg;
        cfg.profile = profile;

        switch(profile)
        {
            case VulnProfile::Easy:
                cfg.idor_enabled = true;
                cfg.weak_sessions = true;
                cfg.race_window_enabled = true;
                cfg.unsafe_input_parsing = true;
                cfg.weak_passwords = true;
                cfg.debug_endpoints = true;
                break;

            case VulnProfile::Medium:
                cfg.idor_enabled = true;
                cfg.weak_sessions = false;
                cfg.race_window_enabled = true;
                cfg.unsafe_input_parsing = true;
                cfg.weak_passwords = true;
                cfg.debug_endpoints = false;
                break;

            case VulnProfile::Hard:
                cfg.idor_enabled = false;
                cfg.weak_sessions = false;
                cfg.race_window_enabled = false;
                cfg.unsafe_input_parsing = false;
                cfg.weak_passwords = false;
                cfg.debug_endpoints = false;
                break;
        }

        return cfg;
    }
}