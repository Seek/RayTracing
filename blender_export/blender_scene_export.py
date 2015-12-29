import bpy
import bmesh
import gzip

# Date 12/28/15
# Wrriten for Blender ver. 2.76.2b
# This file will serve as a record of how we will transmit data from blender to our raytracer.
# These parameters are more or less mirrored information about BlendData types from:
# http://www.blender.org/api/blender_python_api_2_76_2/bpy.types.BlendData.html#bpy.types.BlendData

# TODO: Accept cmd line params for controlling output

scene = bpy.data.scenes[0]

def matrix_to_string(matrix):
    val = ""
    rowlen = len(matrix.row)
    collen = len(matrix.col)
    for i in range(collen):
        for j in range(rowlen):
            if j != (rowlen - 1):
                val += '{0},'.format(matrix[i][j])
            else:
                val += '{0}'.format(matrix[i][j])
        val += '\n'
    return val

def is_mesh_triagulated(mesh):
    pass

# Get the current filename
CURRENT_FILE = bpy.path.basename(bpy.context.blend_data.filepath)
tmpp = CURRENT_FILE.find('.')
FILENAME = CURRENT_FILE[:tmpp] + '_rt.txt'
print(FILENAME)

# Get only the data we're interested in
cams = bpy.data.cameras
lights = bpy.data.lamps
meshes = bpy.data.meshes

buf = ''
# Export lights
# TODO Environment maps?
with open(FILENAME, 'wt') as f:
    buf += "[Lights]\n"
    f.write(buf)
    for l in lights:
        buf = ''
        lo = bpy.data.objects[l.name]
        buf += 'begin light\n'
        buf += 'name={0}\n'.format(l.name)
        buf += 'type={0}\n'.format(l.type)
        buf += 'color={0},{1},{2}\n'.format(l.color.r,
                                            l.color.g, l.color.b)
        buf += 'position={0},{1},{2}\n'.format(lo.location.x, lo.location.y,
                                               lo.location.z)
        buf += 'energy={0}\n'.format(l.energy)
        buf += 'distance={0}\n'.format(l.distance)
        buf += 'object_to_world={0}'.format(matrix_to_string(lo.matrix_world))
        if l.type == 'HEMI':
            pass
        elif l.type == 'POINT':
            pass
        elif l.type == 'AREA':
            buf += 'shape={0}\n'.format(l.shape)
            buf += 'sizex={0}\n'.format(l.size)
            buf += 'sizey={0}\n'.format(l.size_y)
        elif l.type == 'SPOT':
            buf += 'spot_size={0}\n'.format(l.spot_size)
            buf += 'spot_blend={0}\n'.format(l.spot_blend)
        elif l.type == 'SUN':
            pass
        buf += 'end light\n'
        f.write(buf)
    for m in meshes:
        apply_modifiers = True
        settings = 'PREVIEW'
        buf = ''
        mo = bpy.data.objects[m.name]
        mesh = mo.to_mesh(scene, apply_modifiers, settings)
        bm = bmesh.new()
        bm.from_mesh(m)
        bmesh.ops.triangulate(bm, faces=bm.faces)
        bm.free()
        bpy.data.meshes.remove(mesh)
