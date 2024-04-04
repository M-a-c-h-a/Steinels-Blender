import bpy

# Set the rocket's location and orientation
rocket = bpy.data.objects.get('Rocket')
if rocket:
    rocket.location = (0, 0, 10)
    rocket.rotation_euler = (0, 0, 0)

# A placeholder function to simulate receiving real-time data
def get_telemetry_data():
    # Replace this with your actual code that receives the real-time data
    return (1, 2, 3)

# Set up the custom driver to control the rocket's orientation
def custom_driver(x, y, z):
    return x * 0.1

bpy.app.driver_namespace["my_custom_driver"] = custom_driver

# Set up the handler to update the object's location and orientation every frame
def update_rocket(scene):
    if rocket:
        x, y, z = get_telemetry_data()
        rocket.location = (x, y, z)
        rocket.rotation_euler = (0, 0, custom_driver(x, y, z))

# Remove existing handlers to avoid duplicates
bpy.app.handlers.frame_change_pre.clear()

# Set up the handler to be called every frame
bpy.app.handlers.frame_change_pre.append(update_rocket)


bpy.ops.screen.animation_play()