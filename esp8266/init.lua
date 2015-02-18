--print("init.lua for moodbeams")
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","PASSWORD")

-- tmr.alarm(0, 1000, 0, 
	-- function()
		-- print("moodbeams v0.1 - ready")
		dofile("moodbeams.lua")
	-- end
-- )
