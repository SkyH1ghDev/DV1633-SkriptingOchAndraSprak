-- player_update.lua 
local move_speed = 200.0 -- acceleration
local jump_force = 450.0 -- force applied when jumping
local max_h_speed = 400.0 -- maximum horizontal speed
local max_v_speed = 1000.0 -- maximum vertical speed
local friction_factor = 0.6;

function MoveHorizontal(v_h, step)
    -- determine if moving right or left
    local dir = 1
    if step < 0 then
        dir = -1
    end

    -- apply acceleration
    v_h = v_h + step 

    -- limit horizontal speed to not be too big or small
    if v_h > max_h_speed then
        v_h = max_h_speed * dir
    elseif v_h < move_speed then
        v_h = move_speed * dir
    end

    return v_h
end

function ApplyFriction(v)
    -- apply friction based on the horizontal velocity
    -- higher velocity -> more friction
    if v > 0 then
        v = v - sqrt(move_speed) * raylib.GetDelta()
        if v < 0 then
            v = 0
        end
    elseif v < 0 then
        v = v + sqrt(move_speed) * raylib.GetDelta()
        if v > 0 then
            v = 0
        end
    end
return v
end

-- for debugging/testing
function AddPlatform(x, y, width, height)
    -- Add a platform at the specified position and size
    -- This function is called from C++ to add platforms for testing
    update.add_platform(x, y, width, height)
end
-- update player's position
function UpdatePlayer(player_id)
    -- Get current velocity and grounded status from C++
    local vx, vy = update.get_velocity(player_id)
    local is_grounded = update.get_grounded_status(player_id)
    -- local jumped = get_jump_status(player_id)
    local delta = raylib.GetDelta();
    local h_step = move_speed * delta;

    -- Horizontal movement
    -- Input
    if update.is_key_down(keys.RIGHT) or update.is_key_down(keys.D) then
        vx = MoveHorizontal(vx, h_step)
    elseif update.is_key_down(keys.LEFT) or update.is_key_down(keys.A) then
        vx = MoveHorizontal(vx, -h_step)
    else
        -- Braking/friction
        if is_grounded then
            if vx > 0 then
                vx = vx - move_speed^2 * delta
                if vx < 0 then
                    vx = 0
                end
            elseif vx < 0 then
                vx = vx + move_speed^2 * delta
                if vx > 0 then
                    vx = 0
                end
            end
        else
            vx = 0
        end
    end


    -- Y-axis movement
    if update.is_key_pressed(keys.SPACE) and is_grounded then
        vy = vy -jump_force
        if vy < -max_v_speed then
            vy = -max_v_speed
        end
        update.set_grounded_status(player_id, false)
    elseif vy > max_v_speed then
        vy = max_v_speed
    end

    -- Update player velocity inside of C++ to the new one
    update.set_velocity(player_id, vx, vy)

    if update.is_key_pressed(keys.P) then
        -- add a random platform
        local x = math.random(0, 800)
        local y = math.random(100, 600)
        local width = math.random(50, 200)
        local height = math.random(10, 50)

        -- clamp values so they align with the grid (e.g., 50x50)
        -- width or height have to be at least 50
        x = math.floor(x / 50) * 50
        y = math.floor(y / 50) * 50
        width = math.floor(width / 50) * 50
        height = math.floor(height / 50) * 50
        if width < 50 then
            width = 50
        end
        if height < 50 then
            height = 50
        end
        

        AddPlatform(x, y, width, height)
        print("Added platform at (" .. x .. ", " .. y .. ") with size (" .. width .. ", " .. height .. ")")
    end
end