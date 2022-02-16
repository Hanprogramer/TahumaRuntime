Game = {time = 0}
function Game.init()
	-- Run once when the game started
	set_game_title("Auto Farm King by Hanprogramer");
	Game.spr_logo = load_texture("afklogonew.png")
end

function Game.update(delta)
	-- Run every frame
	Game.time = math.floor(Game.time + delta)
end

function Game.draw()
	-- Run only when need redraw
	for y = 0, 100, 1 do
		for x = 0, 100, 1 do
			draw_texture(Game.spr_logo, (x*32)+Game.time,y*32,32,32);
		end
	end
end

function Game.dispose()
	-- Run before everything closes
end

Game.init()