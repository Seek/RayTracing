import bpy, bmesh
import gzip

#This file will serve as a record of how we will transmit data from blender to our raytracer.


#Get the current filename
CURRENT_FILE = bpy.path.basename(bpy.context.blend_data.filepath)

cams = bpy.data.cameras
lights = bpy.data.lamps
meshes = bpy.data.meshes

lightOutput = str()
#Export lights
lightOutput += "[Lights]\n"
for l in lights:
	lo = bpy.data.objects[l.name]
	lightOutput += 'begin light\n'
	lightOutput += 'name={0}\n'.format(l.name)
	lightOutput += 'type={0}\n'.format(l.type)
	lightOutput += 'color={0},{1},{2}\n'.format(l.color.r,l.color.g,l.color.b)
	lightOutput += 'position={0},{1},{2}\n'.format(lo.location.x, lo.location.y, lo.location.z)
	lightOutput += 'energy={0}\n'.format(l.energy)
	lightOutput += 'distance={0}\n'.format(l.distance)
	lightOutput += 'end light\n'

print(lightOutput)
#Export meshes

#Export camera/s?
