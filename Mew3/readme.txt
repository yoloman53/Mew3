-chesterfield92 private multi-
Done:
- triggerbot
- bhop
- read config

ToDo:
- signatures and netvars automatically
- aimbot & smooth aimbot
- bomb defusal:
        if (Bomb->m_hBombDefuser() > 0)
        {
            float countdown = Bomb->m_flDefuseCountDown() - (local->m_nTickBase() * g_GlobalVars->interval_per_tick);




Aimbot:
https://www.unknowncheats.me/wiki/Counter_Strike_Global_Offensive:Coding_a_Better_Aimbot
The first thing we need to do is get our local player's eye position in the world. This is pretty easy. 
You just need to get the origin vector and add that to the view offset. Then you must find the head position of the enemy. 
External users: You will need to iterate the bone struct and find the bone you're looking for. There are multiple threads on this.
Hint: The head bone ID for players is 8