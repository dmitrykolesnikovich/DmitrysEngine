/* Copyright (c) 2017-2019 Dmitry Stepanov a.k.a mr.DIMAS
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

/**
 * ASCII + Binary FBX Loader
 **/

#define DE_FBX_VERBOSE 0

typedef int de_fbx_index_t;

#define DE_FBX_TIME_UNIT (1.0 / 46186158000.0)

typedef struct de_fbx_model_t de_fbx_model_t;

typedef struct de_fbx_key_frame
{
	float time; /* in seconds */
	de_vec3_t position;
	de_vec3_t scale;
	de_vec3_t rotation; /* euler angles */
} de_fbx_key_frame_t;

typedef struct de_fbx_time_value_pair_t
{
	float time;
	float value;
} de_fbx_time_value_pair_t;

typedef struct de_fbx_sub_deformer_t
{
	de_fbx_model_t* model;
	DE_ARRAY_DECLARE(int, indices);
	DE_ARRAY_DECLARE(float, weights);
	de_mat4_t transform;
	de_mat4_t transform_link;
} de_fbx_sub_deformer_t;

typedef struct de_fbx_deformer_t
{
	DE_ARRAY_DECLARE(de_fbx_sub_deformer_t*, sub_deformers);
} de_fbx_deformer_t;

typedef struct de_fbx_animation_curve_t
{
	/* Array of de_fbx_time_value_pair_t */
	DE_ARRAY_DECLARE(de_fbx_time_value_pair_t, keys);
} de_fbx_animation_curve_t;

typedef enum de_fbx_animation_curve_node_type_t
{
	DE_FBX_ANIMATION_CURVE_NODE_UNKNOWN,
	DE_FBX_ANIMATION_CURVE_NODE_TRANSLATION,
	DE_FBX_ANIMATION_CURVE_NODE_ROTATION,
	DE_FBX_ANIMATION_CURVE_NODE_SCALE
} de_fbx_animation_curve_node_type_t;

typedef struct de_fbx_animation_curve_node_t
{
	de_fbx_animation_curve_node_type_t type;
	DE_ARRAY_DECLARE(de_fbx_animation_curve_t*, curves);
} de_fbx_animation_curve_node_t;

typedef struct de_fbx_texture_t
{
	char* filename;
} de_fbx_texture_t;

typedef struct de_fbx_material_t
{
	de_fbx_texture_t* diffuse_tex;
} de_fbx_material_t;

typedef struct de_fbx_geom_t
{
	de_vec3_t* vertices;
	int vertex_count;

	int* indices;
	int index_count;

	de_vec3_t* normals;
	int normal_count;
	de_fbx_mapping_t normal_mapping;
	de_fbx_reference_t normal_reference;

	de_vec3_t* tangents;
	int tangent_count;
	de_fbx_mapping_t tangent_mapping;
	de_fbx_reference_t tangent_reference;

	de_vec3_t* binormals;
	int binormal_count;
	de_fbx_mapping_t binormal_mapping;
	de_fbx_reference_t binormal_reference;

	de_vec2_t* uvs;
	int uv_count;
	int* uv_index;
	int uv_index_count;
	de_fbx_mapping_t uv_mapping;
	de_fbx_reference_t uv_reference;

	int* materials;
	int material_count;
	de_fbx_mapping_t material_mapping;
	de_fbx_reference_t material_reference;

	/* Array of de_fbx_deformer_t */
	DE_ARRAY_DECLARE(de_fbx_deformer_t*, deformers);
} de_fbx_geom_t;

typedef struct de_fbx_light_t
{
	de_color_t color;
	float radius;
} de_fbx_light_t;

struct de_fbx_model_t
{
	char* name;

	de_vec3_t pre_rotation;
	de_vec3_t post_rotation;
	de_vec3_t rotation_offset;
	de_vec3_t rotation_pivot;
	de_vec3_t scaling_offset;
	de_vec3_t scaling_pivot;
	de_vec3_t rotation;
	de_vec3_t scale;
	de_vec3_t translation;
	de_vec3_t geometric_translation;
	de_vec3_t geometric_rotation;
	de_vec3_t geometric_scale;

	/* Used to link fbx node with engine node */
	de_node_t* engine_node;

	DE_ARRAY_DECLARE(de_fbx_geom_t*, geoms);
	DE_ARRAY_DECLARE(de_fbx_material_t*, materials);
	DE_ARRAY_DECLARE(de_fbx_animation_curve_node_t*, animation_curve_nodes);
	DE_ARRAY_DECLARE(de_fbx_model_t*, children);

	/* Components */
	de_fbx_light_t* light;
};

typedef enum de_fbx_component_type_t
{
	DE_FBX_COMPONENT_DEFORMER,
	DE_FBX_COMPONENT_SUB_DEFORMER,
	DE_FBX_COMPONENT_TEXTURE,
	DE_FBX_COMPONENT_LIGHT,
	DE_FBX_COMPONENT_MODEL,
	DE_FBX_COMPONENT_MATERIAL,
	DE_FBX_COMPONENT_ANIMATION_CURVE_NODE,
	DE_FBX_COMPONENT_ANIMATION_CURVE,
	DE_FBX_COMPONENT_GEOM
} de_fbx_component_type_t;

/* Variant component type */
typedef struct de_fbx_component_t
{
	de_fbx_index_t index;
	de_fbx_component_type_t type;
	union
	{
		de_fbx_geom_t geom;
		de_fbx_model_t model;
		de_fbx_texture_t texture;
		de_fbx_material_t material;
		de_fbx_light_t light;
		de_fbx_animation_curve_t animation_curve;
		de_fbx_animation_curve_node_t animation_curve_node;
		de_fbx_sub_deformer_t sub_deformer;
		de_fbx_deformer_t deformer;
	} s;
} de_fbx_component_t;

typedef struct de_fbx_t
{
	DE_ARRAY_DECLARE(de_fbx_component_t*, components);
} de_fbx_t;

#include "fbx/impl/fbx_node.h"
#include "fbx/impl/fbx_ascii.h"
#include "fbx/impl/fbx_binary.h"

/*=======================================================================================*/
static int de_fbx_fix_index(int index)
{
	/* We have to convert last vertex index of face it’s because the original index is XOR’ed with -1 */
	return index < 0 ? -index - 1 : index;
}

/*=======================================================================================*/
static de_fbx_mapping_t de_fbx_get_mapping(const char* str)
{
	if (strcmp(str, "ByPolygon") == 0)
	{
		return DE_FBX_MAPPING_BY_POLYGON;
	}
	else if (strcmp(str, "ByPolygonVertex") == 0)
	{
		return DE_FBX_MAPPING_BY_POLYGON_VERTEX;
	}
	else if (strcmp(str, "ByVertex") == 0 || strcmp(str, "ByVertice") == 0)
	{
		return DE_FBX_MAPPING_BY_VERTEX;
	}
	else if (strcmp(str, "ByEdge") == 0)
	{
		return DE_FBX_MAPPING_BY_EDGE;
	}
	else if (strcmp(str, "AllSame") == 0)
	{
		return DE_FBX_MAPPING_ALL_SAME;
	}
	return DE_FBX_MAPPING_UNKNOWN;
}

/*=======================================================================================*/
static void de_fbx_read_normals(de_fbx_node_t* geom_node, de_fbx_geom_t* geom)
{
	int i, k;
	de_fbx_node_t* normals_node = de_fbx_node_find_child(geom_node, "LayerElementNormal");
	de_fbx_node_t* map_type = de_fbx_node_find_child(normals_node, "MappingInformationType");
	de_fbx_node_t* ref_type = de_fbx_node_find_child(normals_node, "ReferenceInformationType");
	de_fbx_node_t* normals = de_fbx_node_find_child(de_fbx_node_find_child(normals_node, "Normals"), "a");

	geom->normal_mapping = de_fbx_get_mapping(map_type->attributes.data[0]);
	geom->normal_reference = de_fbx_get_reference(ref_type->attributes.data[0]);
	geom->normal_count = normals->attributes.size / 3;
	geom->normals = (de_vec3_t*)de_calloc(geom->normal_count, sizeof(*geom->normals));

	for (i = 0, k = 0; i < geom->normal_count; ++i, k += 3)
	{
		de_fbx_get_vec3(normals, k, &geom->normals[i]);
	}
}

/*=======================================================================================*/
static void de_fbx_read_faces(de_fbx_node_t* geom_node, de_fbx_geom_t* geom)
{
	int i;
	de_fbx_node_t* index_array;

	index_array = de_fbx_node_find_child(de_fbx_node_find_child(geom_node, "PolygonVertexIndex"), "a");

	geom->index_count = index_array->attributes.size;
	geom->indices = (int*)de_calloc(geom->index_count, sizeof(*geom->indices));

	for (i = 0; i < geom->index_count; ++i)
	{
		geom->indices[i] = de_fbx_get_int(index_array, i);
	}
}

/*=======================================================================================*/
static void de_fbx_read_vertices(de_fbx_node_t* geom_node, de_fbx_geom_t* geom)
{
	int i, k;
	de_fbx_node_t* vertex_array;

	vertex_array = de_fbx_node_find_child(de_fbx_node_find_child(geom_node, "Vertices"), "a");

	geom->vertex_count = vertex_array->attributes.size / 3;
	geom->vertices = (de_vec3_t*)de_calloc(geom->vertex_count, sizeof(*geom->vertices));

	for (i = 0, k = 0; i < geom->vertex_count; ++i, k += 3)
	{
		de_fbx_get_vec3(vertex_array, k, &geom->vertices[i]);
	}
}

/*=======================================================================================*/
static void de_fbx_read_uvs(de_fbx_node_t* geom_node, de_fbx_geom_t* geom)
{
	int i, k;
	size_t j;
	de_fbx_node_t* uvs_node = de_fbx_node_find_child(geom_node, "LayerElementUV");

	if (!uvs_node)
	{
		return;
	}

	de_fbx_node_t* map_type = de_fbx_node_find_child(uvs_node, "MappingInformationType");
	de_fbx_node_t* ref_type = de_fbx_node_find_child(uvs_node, "ReferenceInformationType");
	de_fbx_node_t* uvs = de_fbx_node_find_child(de_fbx_node_find_child(uvs_node, "UV"), "a");

	geom->uv_mapping = de_fbx_get_mapping(map_type->attributes.data[0]);
	geom->uv_reference = de_fbx_get_reference(ref_type->attributes.data[0]);
	geom->uv_count = uvs->attributes.size / 2;
	geom->uvs = (de_vec2_t*)de_calloc(geom->uv_count, sizeof(*geom->uvs));

	/* Read uvs */
	for (i = 0, k = 0; i < geom->uv_count; ++i, k += 2)
	{
		de_fbx_get_vec2(uvs, k, &geom->uvs[i]);
	}

	/* Read mapping indices */
	if (geom->uv_reference == DE_FBX_REFERENCE_INDEX_TO_DIRECT)
	{
		de_fbx_node_t* uv_index = de_fbx_node_find_child(de_fbx_node_find_child(uvs_node, "UVIndex"), "a");

		geom->uv_index_count = uv_index->attributes.size;
		geom->uv_index = (int*)de_malloc(geom->uv_index_count * sizeof(*geom->uv_index));

		for (j = 0; j < uv_index->attributes.size; ++j)
		{
			geom->uv_index[j] = de_fbx_get_int(uv_index, j);
		}
	}
}

/*=======================================================================================*/
static void de_fbx_read_geom_materials(de_fbx_node_t* geom_node, de_fbx_geom_t* geom)
{
	size_t i;
	de_fbx_node_t* material_node = de_fbx_node_get_child(geom_node, "LayerElementMaterial");
	if (material_node)
	{
		de_fbx_node_t* map_type = de_fbx_node_find_child(material_node, "MappingInformationType");
		de_fbx_node_t* ref_type = de_fbx_node_find_child(material_node, "ReferenceInformationType");
		de_fbx_node_t* materials = de_fbx_node_find_child(de_fbx_node_find_child(material_node, "Materials"), "a");

		geom->material_mapping = de_fbx_get_mapping(map_type->attributes.data[0]);
		geom->material_reference = de_fbx_get_reference(ref_type->attributes.data[0]);

		geom->material_count = materials->attributes.size;
		geom->materials = (int*)de_malloc(geom->material_count * sizeof(*geom->materials));
		for (i = 0; i < materials->attributes.size; ++i)
		{
			geom->materials[i] = de_fbx_get_int(materials, i);
		}
	}
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_alloc_component(de_fbx_component_type_t type, int index)
{
	de_fbx_component_t* comp = DE_NEW(de_fbx_component_t);
	comp->type = type;
	comp->index = index;
	return comp;
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_geometry(de_fbx_node_t* geom_node)
{
	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_GEOM, de_fbx_get_int(geom_node, 0));
	de_fbx_geom_t* geom = &comp->s.geom;

	de_fbx_read_vertices(geom_node, geom);
	de_fbx_read_faces(geom_node, geom);
	de_fbx_read_normals(geom_node, geom);
	de_fbx_read_uvs(geom_node, geom);
	de_fbx_read_geom_materials(geom_node, geom);

	return comp;
}

/*=======================================================================================*/
static void de_fbx_geom_free(de_fbx_geom_t* geom)
{
	de_free(geom->vertices);
	de_free(geom->binormals);
	de_free(geom->normals);
	de_free(geom->indices);
	de_free(geom->tangents);
	de_free(geom->uvs);
	de_free(geom->uv_index);
	de_free(geom->materials);
	DE_ARRAY_FREE(geom->deformers);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_model(de_fbx_node_t* model_node)
{
	size_t i;
	de_fbx_model_t* model;
	de_fbx_node_t* props;
	de_fbx_component_t* comp;
	char* name;
	char* name_separator;

	comp = de_fbx_alloc_component(DE_FBX_COMPONENT_MODEL, de_fbx_get_int(model_node, 0));

	model = &comp->s.model;

	de_vec3_set(&model->scale, 1, 1, 1);
	de_vec3_set(&model->geometric_scale, 1, 1, 1);

	/* Extract node name */
	name = model_node->attributes.data[1];
	name_separator = strrchr(name, ':');
	if (name_separator)
	{
		model->name = de_str_copy(name_separator + 1);
	}
	else
	{
		model->name = de_str_copy(name);
	}

	props = de_fbx_node_find_child(model_node, "Properties70");

	for (i = 0; i < props->children.size; ++i)
	{
		de_fbx_node_t* prop = props->children.data[i];
		char* property_name = prop->attributes.data[0];

		if (strcmp(property_name, "Lcl Translation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->translation);
		}
		else if (strcmp(property_name, "Lcl Rotation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->rotation);
		}
		else if (strcmp(property_name, "Lcl Scaling") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->scale);
		}
		else if (strcmp(property_name, "PreRotation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->pre_rotation);
		}
		else if (strcmp(property_name, "PostRotation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->post_rotation);
		}
		else if (strcmp(property_name, "RotationOffset") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->rotation_offset);
		}
		else if (strcmp(property_name, "RotationPivot") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->rotation_pivot);
		}
		else if (strcmp(property_name, "ScalingOffset") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->scaling_offset);
		}
		else if (strcmp(property_name, "ScalingPivot") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->scaling_pivot);
		}
		else if (strcmp(property_name, "GeometricTranslation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->geometric_translation);
		}
		else if (strcmp(property_name, "GeometricScaling") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->geometric_scale);
		}
		else if (strcmp(property_name, "GeometricRotation") == 0)
		{
			de_fbx_get_vec3(prop, 4, &model->geometric_rotation);
		}
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_model_free(de_fbx_model_t* model)
{
	de_free(model->name);
	DE_ARRAY_FREE(model->geoms);
	DE_ARRAY_FREE(model->materials);
	DE_ARRAY_FREE(model->animation_curve_nodes);
}

/*=======================================================================================*/
static int de_fbx_component_index_comparer(const void* a, const void* b)
{
	const de_fbx_component_t* comp_a = *(de_fbx_component_t**)a;
	const de_fbx_component_t* comp_b = *(de_fbx_component_t**)b;
	if (comp_a->index < comp_b->index)
	{
		return -1;
	}
	if (comp_a->index == comp_b->index)
	{
		return 0;
	}
	return 1;
}

/*=======================================================================================*/
static void de_fbx_sort_components_by_index(de_fbx_t* fbx)
{
	DE_ARRAY_QSORT(fbx->components, de_fbx_component_index_comparer);
}

/*=======================================================================================*/
static int de_fbx_component_bsearch_comparer(const void* key_ptr, const void* comp_ptr)
{
	const de_fbx_index_t index = *(de_fbx_index_t*)key_ptr;
	const de_fbx_component_t* comp = *(de_fbx_component_t**)comp_ptr;
	if (index < comp->index)
	{
		return -1;
	}
	if (index == comp->index)
	{
		return 0;
	}
	return 1;
}

static de_fbx_component_t* de_fbx_find_component(de_fbx_t* fbx, int index)
{
	void** comp = (void**)DE_ARRAY_BSEARCH(fbx->components, &index, de_fbx_component_bsearch_comparer);
	return comp ? (de_fbx_component_t*)(*comp) : NULL;
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_material(de_fbx_node_t* mat_node)
{
	return de_fbx_alloc_component(DE_FBX_COMPONENT_MATERIAL, de_fbx_get_int(mat_node, 0));
}

/*=======================================================================================*/
static void de_fbx_material_free(de_fbx_material_t* mat)
{
	DE_UNUSED(mat);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_texture(de_fbx_node_t* tex_node)
{
	de_fbx_texture_t* tex;
	char* path;
	char* filename;

	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_TEXTURE, de_fbx_get_int(tex_node, 0));

	tex = &comp->s.texture;

	path = de_fbx_node_find_child(tex_node, "RelativeFilename")->attributes.data[0];
	filename = strrchr(path, '\\');

	if (filename)
	{
		tex->filename = de_str_copy(filename + 1);
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_texture_free(de_fbx_texture_t* tex)
{
	de_free(tex->filename);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_light(de_fbx_node_t* light_node)
{
	size_t i;
	de_fbx_light_t* light;
	de_fbx_node_t* props;

	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_LIGHT, de_fbx_get_int(light_node, 0));

	light = &comp->s.light;

	props = de_fbx_node_find_child(light_node, "Properties70");

	de_color_set(&light->color, 255, 255, 255, 255);

	for (i = 0; i < props->children.size; ++i)
	{
		de_fbx_node_t* child;
		char* prop_name;

		child = props->children.data[i];

		prop_name = child->attributes.data[0];
		if (strcmp(prop_name, "DecayStart") == 0)
		{
			light->radius = de_fbx_get_double(child, 4);
		}
		else if (strcmp(prop_name, "Color") == 0)
		{
			float r, g, b;

			if (child->attributes.size > 5)
			{
				r = de_fbx_get_double(child, 4);
				g = de_fbx_get_double(child, 5);
				b = de_fbx_get_double(child, 6);

				light->color.r = (unsigned char)(255 * r);
				light->color.g = (unsigned char)(255 * g);
				light->color.b = (unsigned char)(255 * b);
				light->color.a = 255;
			}
		}
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_light_free(de_fbx_light_t* light)
{
	DE_UNUSED(light);
}

/*=======================================================================================*/
/* Reads animation curve from AnimationCurve node */
static de_fbx_component_t* de_fbx_read_animation_curve(de_fbx_node_t* anim_curve_node)
{
	size_t i;
	de_fbx_animation_curve_t* curve;
	de_fbx_node_t* key_time;
	de_fbx_node_t* key_time_array;
	de_fbx_node_t* key_value;
	de_fbx_node_t* key_value_array;

	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_ANIMATION_CURVE, de_fbx_get_int(anim_curve_node, 0));

	curve = &comp->s.animation_curve;

	key_time = de_fbx_node_find_child(anim_curve_node, "KeyTime");
	key_value = de_fbx_node_find_child(anim_curve_node, "KeyValueFloat");

	if (key_value == NULL || key_time == NULL)
	{
		de_error("FBX: KeyTime or KeyValueFloat is missing");
	}

	key_time_array = de_fbx_node_find_child(key_time, "a");
	key_value_array = de_fbx_node_find_child(key_value, "a");

	if (key_time_array->attributes.size != key_value_array->attributes.size)
	{
		de_error("FBX: Animation curve contains wrong key data!");
	}

	for (i = 0; i < key_time_array->attributes.size; ++i)
	{
		de_fbx_time_value_pair_t pair;

		pair.time = (float)(de_fbx_get_int64(key_time_array, i) * DE_FBX_TIME_UNIT);
		pair.value = de_fbx_get_float(key_value_array, i);

		DE_ARRAY_APPEND(curve->keys, pair);
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_animation_curve_free(de_fbx_animation_curve_t* curve)
{
	DE_ARRAY_FREE(curve->keys);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_animation_curve_node_t(de_fbx_node_t* anim_curve_fbx_node)
{
	de_fbx_animation_curve_node_t* anim_curve_node;
	char* type_str;

	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_ANIMATION_CURVE_NODE, de_fbx_get_int(anim_curve_fbx_node, 0));

	anim_curve_node = &comp->s.animation_curve_node;

	type_str = anim_curve_fbx_node->attributes.data[1];

	if (strcmp(type_str, "T") == 0 || strcmp(type_str, "AnimCurveNode::T") == 0)
	{
		anim_curve_node->type = DE_FBX_ANIMATION_CURVE_NODE_TRANSLATION;
	}
	else if (strcmp(type_str, "R") == 0 || strcmp(type_str, "AnimCurveNode::R") == 0)
	{
		anim_curve_node->type = DE_FBX_ANIMATION_CURVE_NODE_ROTATION;
	}
	else if (strcmp(type_str, "S") == 0 || strcmp(type_str, "AnimCurveNode::S") == 0)
	{
		anim_curve_node->type = DE_FBX_ANIMATION_CURVE_NODE_SCALE;
	}
	else
	{
		de_log("FBX: Unknown %s animation curve node type!", type_str);
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_animation_curve_node_free(de_fbx_animation_curve_node_t* curve_node)
{
	DE_ARRAY_FREE(curve_node->curves);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_sub_deformer(de_fbx_node_t* sub_deformer_node)
{
	size_t i;
	de_fbx_sub_deformer_t* sub_deformer;
	de_fbx_node_t* indices;
	de_fbx_node_t* weights;
	de_fbx_node_t* transform;
	de_fbx_node_t* transform_link;

	de_fbx_component_t* comp = de_fbx_alloc_component(DE_FBX_COMPONENT_SUB_DEFORMER, de_fbx_get_int(sub_deformer_node, 0));

	sub_deformer = &comp->s.sub_deformer;

	indices = de_fbx_node_find_child(de_fbx_node_find_child(sub_deformer_node, "Indexes"), "a");
	weights = de_fbx_node_find_child(de_fbx_node_find_child(sub_deformer_node, "Weights"), "a");
	transform = de_fbx_node_find_child(de_fbx_node_find_child(sub_deformer_node, "Transform"), "a");
	transform_link = de_fbx_node_find_child(de_fbx_node_find_child(sub_deformer_node, "TransformLink"), "a");

	if (transform->attributes.size != 16)
	{
		de_error("FBX: Wrong transform size! Expect 16, got %d", transform->attributes.size);
	}
	if (transform_link->attributes.size != 16)
	{
		de_error("FBX: Wrong transform link size! Expect 16, got %d", transform_link->attributes.size);
	}

	for (i = 0; i < indices->attributes.size; ++i)
	{
		int index = de_fbx_get_int(indices, i);
		DE_ARRAY_APPEND(sub_deformer->indices, index);
	}

	for (i = 0; i < weights->attributes.size; ++i)
	{
		float weight = de_fbx_get_float(weights, i);
		DE_ARRAY_APPEND(sub_deformer->weights, weight);
	}

	for (i = 0; i < 16; ++i)
	{
		sub_deformer->transform.f[i] = de_fbx_get_float(transform, i);
	}
	for (i = 0; i < 16; ++i)
	{
		sub_deformer->transform_link.f[i] = de_fbx_get_float(transform_link, i);
	}

	return comp;
}

/*=======================================================================================*/
static void de_fbx_sub_deformer_free(de_fbx_sub_deformer_t* sub_deformer)
{
	DE_ARRAY_FREE(sub_deformer->indices);
	DE_ARRAY_FREE(sub_deformer->weights);
}

/*=======================================================================================*/
static de_fbx_component_t* de_fbx_read_deformer(de_fbx_node_t* deformer_node)
{
	return de_fbx_alloc_component(DE_FBX_COMPONENT_DEFORMER, de_fbx_get_int(deformer_node, 0));
}

/*=======================================================================================*/
static void de_fbx_deformer_free(de_fbx_deformer_t* deformer)
{
	DE_ARRAY_FREE(deformer->sub_deformers);
}

/*=======================================================================================*/
static de_fbx_t* de_fbx_read(de_fbx_node_t* root)
{
	size_t i;
	int version_num;
	de_fbx_node_t* objects;
	de_fbx_node_t* connections;
	de_fbx_node_t* header;
	de_fbx_node_t* version;
	de_fbx_t* fbx;

	/* Check header first */
	header = de_fbx_node_find_child(root, "FBXHeaderExtension");
	version = de_fbx_node_find_child(header, "FBXVersion");
	version_num = de_fbx_get_int(version, 0);

	if (version_num < 7100)
	{
		de_log("FBX: Unable to load old FBX %d version. Version MUST be >= 7100", version_num);

		return NULL;
	}

	fbx = DE_NEW(de_fbx_t);

	/* Read all supported objects from fbx and convert it to suitable format */
	objects = de_fbx_node_find_child(root, "Objects");
	for (i = 0; i < objects->children.size; ++i)
	{
		de_fbx_node_t* child = objects->children.data[i];
		if (strcmp(child->name, "Geometry") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_geometry(objects->children.data[i]));
		}
		else if (strcmp(child->name, "Model") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_model(child));
		}
		else if (strcmp(child->name, "Material") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_material(child));
		}
		else if (strcmp(child->name, "Texture") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_texture(child));
		}
		else if (strcmp(child->name, "NodeAttribute") == 0)
		{
			if (child->attributes.size > 2)
			{
				if (strcmp(child->attributes.data[2], "Light") == 0)
				{
					DE_ARRAY_APPEND(fbx->components, de_fbx_read_light(child));
				}
			}
		}
		else if (strcmp(child->name, "AnimationCurve") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_animation_curve(child));
		}
		else if (strcmp(child->name, "AnimationCurveNode") == 0)
		{
			DE_ARRAY_APPEND(fbx->components, de_fbx_read_animation_curve_node_t(child));
		}
		else if (strcmp(child->name, "Deformer") == 0)
		{
			char* type = child->attributes.data[1];
			if (strcmp(type, "SubDeformer::") == 0)
			{
				DE_ARRAY_APPEND(fbx->components, de_fbx_read_sub_deformer(child));
			}
			else if (strcmp(type, "Deformer::") == 0)
			{
				DE_ARRAY_APPEND(fbx->components, de_fbx_read_deformer(child));
			}
		}
	}

	de_fbx_sort_components_by_index(fbx);

#if DE_FBX_VERBOSE
	de_log("FBX: Contains %d geometries (meshes)", fbx->geoms.size);
	de_log("FBX: Contains %d models (nodes)", fbx->models.size);
	de_log("FBX: Contains %d materials", fbx->materials.size);
	de_log("FBX: Contains %d textures", fbx->textures.size);
	de_log("FBX: Contains %d lights", fbx->lights.size);
	de_log("FBX: Contains %d animation curves", fbx->animation_curves.size);
	de_log("FBX: Contains %d animation curve nodes", fbx->animation_curve_nodes.size);
	de_log("FBX: Contains %d deformers", fbx->deformers.size);
	de_log("FBX: Contains %d sub deformers", fbx->sub_deformers.size);
#endif

	/* Read connections and connect components */
	connections = de_fbx_node_find_child(root, "Connections");
	for (i = 0; i < connections->children.size; ++i)
	{
		de_fbx_node_t* child = connections->children.data[i];

		de_fbx_component_t* child_comp = de_fbx_find_component(fbx, de_fbx_get_int(child, 1));
		de_fbx_component_t* parent_comp = de_fbx_find_component(fbx, de_fbx_get_int(child, 2));

		if (!child_comp || !parent_comp)
		{
			continue;
		}

		if (child_comp->type == DE_FBX_COMPONENT_GEOM && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			DE_ARRAY_APPEND(parent_comp->s.model.geoms, &child_comp->s.geom);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_TEXTURE && parent_comp->type == DE_FBX_COMPONENT_MATERIAL)
		{
			parent_comp->s.material.diffuse_tex = &child_comp->s.texture;
		}
		else if (child_comp->type == DE_FBX_COMPONENT_MATERIAL && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			DE_ARRAY_APPEND(parent_comp->s.model.materials, &child_comp->s.material);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_ANIMATION_CURVE && parent_comp->type == DE_FBX_COMPONENT_ANIMATION_CURVE_NODE)
		{
			DE_ARRAY_APPEND(parent_comp->s.animation_curve_node.curves, &child_comp->s.animation_curve);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_ANIMATION_CURVE_NODE && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			DE_ARRAY_APPEND(parent_comp->s.model.animation_curve_nodes, &child_comp->s.animation_curve_node);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_SUB_DEFORMER && parent_comp->type == DE_FBX_COMPONENT_DEFORMER)
		{
			DE_ARRAY_APPEND(parent_comp->s.deformer.sub_deformers, &child_comp->s.sub_deformer);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_DEFORMER && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			DE_ARRAY_APPEND(parent_comp->s.geom.deformers, &child_comp->s.deformer);
		}
		else if (child_comp->type == DE_FBX_COMPONENT_MODEL && parent_comp->type == DE_FBX_COMPONENT_SUB_DEFORMER)
		{
			parent_comp->s.sub_deformer.model = &child_comp->s.model;
		}
		else if (child_comp->type == DE_FBX_COMPONENT_LIGHT && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			parent_comp->s.model.light = &child_comp->s.light;
		}
		else if (child_comp->type == DE_FBX_COMPONENT_MODEL && parent_comp->type == DE_FBX_COMPONENT_MODEL)
		{
			DE_ARRAY_APPEND(parent_comp->s.model.children, &child_comp->s.model);
		}
	}

	return fbx;
}

/*=======================================================================================*/
/**
 * @brief Frees resources associated with fbx structure
 * @param fbx pointer
 */
static void de_fbx_free(de_fbx_t* fbx)
{
	size_t i;
	for (i = 0; i < fbx->components.size; ++i)
	{
		de_fbx_component_t* comp = fbx->components.data[i];
		switch (comp->type)
		{
		case DE_FBX_COMPONENT_ANIMATION_CURVE:
			de_fbx_animation_curve_free(&comp->s.animation_curve);
			break;
		case DE_FBX_COMPONENT_ANIMATION_CURVE_NODE:
			de_fbx_animation_curve_node_free(&comp->s.animation_curve_node);
			break;
		case DE_FBX_COMPONENT_DEFORMER:
			de_fbx_deformer_free(&comp->s.deformer);
			break;
		case DE_FBX_COMPONENT_GEOM:
			de_fbx_geom_free(&comp->s.geom);
			break;
		case DE_FBX_COMPONENT_LIGHT:
			de_fbx_light_free(&comp->s.light);
			break;
		case DE_FBX_COMPONENT_MATERIAL:
			de_fbx_material_free(&comp->s.material);
			break;
		case DE_FBX_COMPONENT_MODEL:
			de_fbx_model_free(&comp->s.model);
			break;
		case DE_FBX_COMPONENT_SUB_DEFORMER:
			de_fbx_sub_deformer_free(&comp->s.sub_deformer);
			break;
		case DE_FBX_COMPONENT_TEXTURE:
			de_fbx_texture_free(&comp->s.texture);
			break;
		}
		de_free(comp);
	}
	DE_ARRAY_FREE(fbx->components);
	de_free(fbx);
}

/*=======================================================================================*/
/**
 * @brief Performs optimized vertex insertion in mesh
 * @param mesh
 * @param new_vertex
 *
 * Main purpose of this routine is to split FBX geometry into set of surfaces with single texture
 * per surface, so engine can perform optimized rendering of such set. This is needed because fbx's
 * vertices data is a total mess - any vertex can have few normals, few texture coordinates and etc.
 * This routine splits such cases so vertex will have one normal, texture coordinate and etc.
 *
 * It must be optimized using hash set for faster search
 *
 * If surface already has vertex, adds index to face list.
 */
static void de_fbx_add_vertex_to_surface(de_surface_t* surf, de_vertex_t* new_vertex)
{
	int i;
	de_bool_t found_identic = DE_FALSE;

	/* reverse search is much faster, because it most likely that we'll find indentic vertex nearby current in the list */
	for (i = (int)surf->vertices.size - 1; i >= 0; --i)
	{
		de_vertex_t* other_vertex = (de_vertex_t*)surf->vertices.data + i;
		if (de_vec3_equals(&other_vertex->position, &new_vertex->position) &&
			de_vec3_equals(&other_vertex->normal, &new_vertex->normal) &&
			de_vec2_equals(&other_vertex->tex_coord, &new_vertex->tex_coord))
		{
			found_identic = DE_TRUE;
			break;
		}
	}

	if (!found_identic)
	{
		i = surf->vertices.size;
		DE_ARRAY_APPEND(surf->vertices, *new_vertex);
	}

	DE_ARRAY_APPEND(surf->indices, i);

	surf->need_upload = DE_TRUE;
}

/*=======================================================================================*/
static void de_fbx_quat_from_euler(de_quat_t* out, const de_vec3_t * euler_angles)
{
	de_vec3_t radians;

	radians.x = de_deg_to_rad(euler_angles->x);
	radians.y = de_deg_to_rad(euler_angles->y);
	radians.z = de_deg_to_rad(euler_angles->z);

	de_quat_from_euler(out, &radians, DE_EULER_XYZ);
}

/*=======================================================================================*/
/**
 * @brief Searches for next closest time to given. When 'current_time' is end time - returns FLT_MAX
 */
static float de_fbx_get_next_keyframe_time(float current_time,
	de_fbx_animation_curve_node_t* t,
	de_fbx_animation_curve_node_t* r,
	de_fbx_animation_curve_node_t* s)
{
	int i;
	size_t k, n;
	float closest_time = FLT_MAX;
	de_fbx_animation_curve_node_t* curve_set[3];

	curve_set[0] = t;
	curve_set[1] = r;
	curve_set[2] = s;

	for (i = 0; i < 3; ++i)
	{
		de_fbx_animation_curve_node_t* node = curve_set[i];

		if (!node)
		{
			continue;
		}

		for (k = 0; k < node->curves.size; ++k)
		{
			de_fbx_animation_curve_t* curve = node->curves.data[k];

			for (n = 0; n < curve->keys.size; ++n)
			{
				float distance;
				de_fbx_time_value_pair_t* key = curve->keys.data + n;

				if (key->time > current_time)
				{
					distance = key->time - current_time;

					if (distance < closest_time - key->time)
					{
						closest_time = key->time;
					}
				}
			}
		}
	}

	return closest_time;
}

/*=======================================================================================*/
/**
 * @brief Evaluates single float value at given time
 */
static float de_fbx_eval_curve(float time, de_fbx_animation_curve_t* curve)
{
	size_t i;
	size_t key_count = curve->keys.size;
	de_fbx_time_value_pair_t* keys;

	if (curve->keys.size == 0)
	{
		de_log("FBX: Trying to evaluate curve with no keys!");

		return 0.0f;
	}

	key_count = curve->keys.size;
	keys = curve->keys.data;

	if (time <= keys[0].time)
	{
		return keys[0].value;
	}

	if (time >= keys[curve->keys.size - 1].time)
	{
		return keys[curve->keys.size - 1].value;
	}

	/* do linear search for span */
	for (i = 0; i < key_count; ++i)
	{
		/* TODO: for now assume that we have only linear transitions */
		if (keys[i].time >= time)
		{
			de_fbx_time_value_pair_t* cur = keys + i;
			de_fbx_time_value_pair_t* next = cur + 1;

			/* calculate interpolation coefficient */
			float time_span = next->time - cur->time;
			float k = (time - cur->time) / time_span;

			/* do linear interpolation */
			float val_span = next->value - cur->value;
			return cur->value + k * val_span;
		}
	}

	/* must be unreached */
	de_log("FBX: How the hell did you get here?!");

	return 0.0f;
}

/*=======================================================================================*/
/**
 * @brief Evaluates curve node as float3 value at give time.
 */
static void de_fbx_eval_float3_curve_node(float time, de_fbx_animation_curve_node_t* node, de_vec3_t* out)
{
	de_fbx_animation_curve_t* x, *y, *z;

	if (node->curves.size != 3)
	{
		de_log("FBX: Attempt to evaluate non-float3 curve node!");
		return;
	}

	/* TODO: Here we making assumption that curves for X Y Z will be in that order. This may be wrong!
	 * From closer look, FBX files always have these curves written in X Y Z order. */
	x = node->curves.data[0];
	y = node->curves.data[1];
	z = node->curves.data[2];

	out->x = de_fbx_eval_curve(time, x);
	out->y = de_fbx_eval_curve(time, y);
	out->z = de_fbx_eval_curve(time, z);
}

/*=======================================================================================*/
/**
 * @brief Merges TRS curves into engine keyframe
 */
static void de_fbx_eval_keyframe(float time,
	de_fbx_animation_curve_node_t* t_node,
	de_fbx_animation_curve_node_t* r_node,
	de_fbx_animation_curve_node_t* s_node,
	de_keyframe_t* out_key)
{
	de_vec3_t euler_angles;

	if (t_node)
	{
		de_fbx_eval_float3_curve_node(time, t_node, &out_key->position);
	}
	else
	{
		de_vec3_set(&out_key->position, 0, 0, 0);
	}

	if (r_node)
	{
		de_fbx_eval_float3_curve_node(time, r_node, &euler_angles);
	}
	else
	{
		de_vec3_set(&euler_angles, 0, 0, 0);
	}
	de_fbx_quat_from_euler(&out_key->rotation, &euler_angles);

	if (s_node)
	{
		de_fbx_eval_float3_curve_node(time, s_node, &out_key->scale);
	}
	else
	{
		de_vec3_set(&out_key->scale, 1, 1, 1);
	}

#if DE_FBX_VERBOSE
	de_log("FBX:Time:%f\n\tT: %f, %f, %f\n\tR: %f, %f, %f\n\tS: %f, %f, %f\n", time,
		out_key->position.x, out_key->position.y, out_key->position.z,
		euler_angles.x, euler_angles.y, euler_angles.z,
		out_key->scale.x, out_key->scale.y, out_key->scale.z);
#endif

	out_key->time = time;
}

/**
 * @brief Reads N indices from 'start' position until negative index is not found.
 * If index count == 3, writes out triangle indices, otherwise performs triangulation
 * of a polygon.
 * @param out_index_count If returned number is negative, then you must skip the face - it is probably corrupted or badly formed.
 * @return Returns number of indices that have been processed.
 *
 * IMPORTANT NOTE: This routine will modify indices array so it will not be contain negative
 * indices! In 99.99% cases this is fine.
 */
static int de_fbx_prepare_next_face(de_fbx_geom_t* geom,
	int start,
	int* out_indices,
	int* out_index_count,
	int* out_relative_indices,
	int out_rel_indices_buffer_size)
{
	int i;
	int vertex_per_face = 0;

	/* count how many vertices are per face */
	for (i = start; i < geom->index_count; ++i)
	{
		++vertex_per_face;

		if (geom->indices[i] < 0)
		{
			geom->indices[i] = de_fbx_fix_index(geom->indices[i]);

			break;
		}
	}

	if (vertex_per_face == 3)
	{
		/* we have a triangle */
		out_indices[0] = geom->indices[start];
		out_indices[1] = geom->indices[start + 1];
		out_indices[2] = geom->indices[start + 2];

		out_relative_indices[0] = 0;
		out_relative_indices[1] = 1;
		out_relative_indices[2] = 2;

		*out_index_count = 3;

		/* check for valid indices here: some triangulators can produce garbage indices.
		 * I encountered this problem in 3ds max 2012 FBX exporter, it just ignores
		 * triangulation checkbox and performs triangulation which fails on non-simple polygons.
		 **/
		for (i = 0; i < 3; ++i)
		{
			if (out_indices[i] > geom->vertex_count)
			{
				/* indicate that processed face is invalid */
				*out_index_count = -1;
				break;
			}
		}
	}
	else if (vertex_per_face > 3)
	{
		/* triangulate a polygon */

		/* fill vertices */
		de_vec3_t* vertices = (de_vec3_t*)de_malloc(vertex_per_face * sizeof(de_vec3_t));
		for (i = 0; i < vertex_per_face; ++i)
		{
			vertices[i] = geom->vertices[geom->indices[start + i]];
		}

		*out_index_count = de_triangulate(vertices, vertex_per_face, out_relative_indices, out_rel_indices_buffer_size);

		if (*out_index_count < 0)
		{
			de_log("FBX: unable to triangulate a polygon. not a simple polygon?");
		}

		/* remap indices back to model */
		for (i = 0; i < *out_index_count; ++i)
		{
			out_indices[i] = geom->indices[start + out_relative_indices[i]];
		}

		de_free(vertices);
	}
	else
	{
		de_log("FBX: vertex per face less than 3???");
	}

	return vertex_per_face;
}

/*=======================================================================================*/
/**
 * @brief Converts FBX to scene engine's scene nodes hierarchy
 * @param fbx
 * @return
 */
static de_node_t* de_fbx_to_scene(de_scene_t* scene, de_fbx_t* fbx)
{
	size_t i, k;
	int n, m;
	int material_index;
	de_node_t* root;
	de_animation_t* anim;
	de_renderer_t* renderer;
	int temp_indices[8192], relative_indices[8192];
	DE_ARRAY_DECLARE(de_node_t*, created_nodes);
	
	renderer = scene->core->renderer;

	root = de_node_create(scene, DE_NODE_BASE);
	de_scene_add_node(scene, root);

	/* Each scene has animation */
	anim = de_animation_create(scene);

	DE_ARRAY_INIT(created_nodes);

	/**
	 * Convert models first.
	 **/
	for (i = 0; i < fbx->components.size; ++i)
	{
		de_fbx_model_t* mdl;
		de_node_t* node;
		de_node_type_t type;
		de_fbx_component_t* comp = fbx->components.data[i];
		de_mat4_t geometric_transform;

		if (comp->type != DE_FBX_COMPONENT_MODEL)
		{
			continue;
		}

		mdl = &comp->s.model;

		if (mdl->light)
		{
			type = DE_NODE_LIGHT;
		}
		else if (mdl->geoms.size)
		{
			type = DE_NODE_MESH;
		}
		else
		{
			type = DE_NODE_BASE;
		}

		node = de_node_create(scene, type);
		de_scene_add_node(scene, node);

		DE_ARRAY_APPEND(created_nodes, node);

		/* save pointer to fbx struct to make further linking easier */
		node->user_data = mdl;
		mdl->engine_node = node;

		node->name = de_str_copy(mdl->name);

		node->position = mdl->translation;
		node->rotation_offset = mdl->rotation_offset;
		node->rotation_pivot = mdl->rotation_pivot;
		node->scaling_offset = mdl->scaling_offset;
		node->scaling_pivot = mdl->scaling_pivot;
		node->scale = mdl->scale;
		de_fbx_quat_from_euler(&node->pre_rotation, &mdl->pre_rotation);
		de_fbx_quat_from_euler(&node->rotation, &mdl->rotation);
		de_fbx_quat_from_euler(&node->post_rotation, &mdl->post_rotation);

		de_node_calculate_transforms(node);

		/* Build geometric transform matrix to bake it into vertex buffer
		 * TODO: not sure if this is right, but according to FBX manual
		 * geometric transform is not inherited and applied directly to
		 * global transform after was calculated.
		 **/
		{
			de_mat4_t t, r, s;
			de_quat_t rq;

			de_mat4_translation(&t, &mdl->geometric_translation);

			de_fbx_quat_from_euler(&rq, &mdl->geometric_rotation);
			de_mat4_rotation(&r, &rq);

			de_mat4_scale(&s, &mdl->geometric_scale);

			de_mat4_mul(&geometric_transform, &t, &r);
			de_mat4_mul(&geometric_transform, &geometric_transform, &s);
		}

		if (mdl->light)
		{
			de_light_t* light = &node->s.light;
			light->color = mdl->light->color;
			light->radius = mdl->light->radius;

			/* FIXME: Add light type support */
			light->type = DE_LIGHT_TYPE_POINT;
		}

		for (k = 0; k < mdl->geoms.size; ++k)
		{
			de_fbx_geom_t* geom;
			de_mesh_t* mesh;

			geom = mdl->geoms.data[k];
			mesh = &node->s.mesh;

			/* Create surfaces per material */
			if (mdl->materials.size == 0)
			{
				de_mesh_add_surface(mesh, de_renderer_create_surface(renderer));
			}
			else
			{
				for (k = 0; k < mdl->materials.size; ++k)
				{
					de_surface_t* surf = de_renderer_create_surface(renderer);
					de_fbx_material_t* mat = mdl->materials.data[k];
					if (mat->diffuse_tex)
					{
						char* filename = de_str_format("data/textures/%s", mat->diffuse_tex->filename);
						de_texture_t* texture = de_renderer_request_texture(renderer, filename);
						de_surface_set_texture(surf, texture);
					}
					de_mesh_add_surface(mesh, surf);
				}
			}

			material_index = 0;
			for (n = 0; n < geom->index_count; )
			{
				int index_per_face;
				int origin = n;

				n += de_fbx_prepare_next_face(geom, n, temp_indices, &index_per_face, relative_indices, sizeof(relative_indices) / sizeof(relative_indices[0]));;

				/* face is invalid, skip it and move to next one */
				if (index_per_face < 0)
				{
					continue;
				}

				for (m = 0; m < index_per_face; ++m)
				{
					de_vertex_t v = { 0 };
					int index = temp_indices[m];
					int surface_index = 0;

					v.position = geom->vertices[index];
					de_vec3_transform(&v.position, &v.position, &geometric_transform);

					/* Extract normal */
					switch (geom->normal_mapping)
					{
					case DE_FBX_MAPPING_BY_POLYGON_VERTEX:
						v.normal = geom->normals[origin + relative_indices[m]];
						break;
					case DE_FBX_MAPPING_BY_VERTEX:
						v.normal = geom->normals[index];
						break;
					default:
						de_error("FBX: Normal mapping is not supported");
					}
					de_vec3_transform_normal(&v.normal, &v.normal, &geometric_transform);

					switch (geom->uv_mapping)
					{
					case DE_FBX_MAPPING_BY_POLYGON_VERTEX:
						if (geom->uv_reference == DE_FBX_REFERENCE_DIRECT)
						{
							v.tex_coord = geom->uvs[origin + relative_indices[m]];
						}
						else if (geom->uv_reference == DE_FBX_REFERENCE_INDEX_TO_DIRECT)
						{
							v.tex_coord = geom->uvs[geom->uv_index[origin + relative_indices[m]]];
						}
						break;
					case DE_FBX_MAPPING_UNKNOWN:
						/* means that there is no uvs */
						break;
					default:
						de_error("FBX: UV mapping is not supported");
					}

					/* Flip texture upside-down */
					v.tex_coord.y = -v.tex_coord.y;

					/* Extract material index. It's used as index of surface in the mesh */
					if (geom->materials)
					{
						switch (geom->material_mapping)
						{
						case DE_FBX_MAPPING_ALL_SAME:
							surface_index = geom->materials[0];
							break;
						case DE_FBX_MAPPING_BY_POLYGON:
							surface_index = geom->materials[material_index];
							break;
						default:
							de_error("FBX: Material mapping is not supported");
						}
					}

					de_fbx_add_vertex_to_surface(mesh->surfaces.data[surface_index], &v);
				}

				if (geom->material_mapping == DE_FBX_MAPPING_BY_POLYGON)
				{
					++material_index;
				}
			}
		}

		/**
		 * Convert animations
		 **/
		if (mdl->animation_curve_nodes.size > 0)
		{
			float time = 0;
			de_animation_track_t* track;
			de_fbx_animation_curve_node_t* lcl_translation = NULL;
			de_fbx_animation_curve_node_t* lcl_rotation = NULL;
			de_fbx_animation_curve_node_t* lcl_scale = NULL;

			/* find supported curve nodes (translation, rotation, scale) */
			for (k = 0; k < mdl->animation_curve_nodes.size; ++k)
			{
				de_fbx_animation_curve_node_t* curve_node = mdl->animation_curve_nodes.data[k];

				if (curve_node->type == DE_FBX_ANIMATION_CURVE_NODE_ROTATION)
				{
					if (!lcl_rotation)
					{
						lcl_rotation = curve_node;
					}
					else
					{
						de_log("FBX: another local rotation curve node?");
					}
				}
				else if (curve_node->type == DE_FBX_ANIMATION_CURVE_NODE_TRANSLATION)
				{
					if (!lcl_translation)
					{
						lcl_translation = curve_node;
					}
					else
					{
						de_log("FBX: another local translation curve node?");
					}
				}
				else if (curve_node->type == DE_FBX_ANIMATION_CURVE_NODE_SCALE)
				{
					if (!lcl_scale)
					{
						lcl_scale = curve_node;
					}
					else
					{
						de_log("FBX: another local scale curve node?");
					}
				}
			}

			/* convert to engine format */
			track = de_animation_track_create(anim);
			de_animation_add_track(anim, track);

			track->node = node;

		#if DE_FBX_VERBOSE
			de_log("FBX: === new anim track ===\n");
		#endif

			/* TODO: Very brittle method, relies on compare two floats! Should be rewritten! */
			for (;;)
			{
				de_keyframe_t keyframe;

				de_fbx_eval_keyframe(time, lcl_translation, lcl_rotation, lcl_scale, &keyframe);

				de_animation_track_add_keyframe(track, &keyframe);

				float next_time = de_fbx_get_next_keyframe_time(time, lcl_translation, lcl_rotation, lcl_scale);

				if (next_time >= FLT_MAX)
				{
					break;
				}

				time = next_time;
			}
		}

		de_node_attach(node, root);
	}

	/* Link nodes according to hierarchy */
	for (i = 0; i < created_nodes.size; ++i)
	{
		de_node_t* node = created_nodes.data[i];
		de_fbx_model_t* fbx_model = (de_fbx_model_t*)node->user_data;

		if (fbx_model->children.size > 0)
		{
			for (k = 0; k < fbx_model->children.size; ++k)
			{
				de_node_t* child = fbx_model->children.data[k]->engine_node;

				de_node_attach(child, node);
			}
		}		
	}

	DE_ARRAY_FREE(created_nodes);

	de_animation_clamp_length(anim);
	anim->speed = 0.1f;

	return root;
}

/*=======================================================================================*/
de_node_t* de_fbx_load_to_scene(de_scene_t* scene, const char* file)
{
	de_fbx_node_t* root;
	de_fbx_t* fbx;
	de_node_t* root_node;
	float last_time;
	de_fbx_buffer_t data_buf;

	last_time = de_time_get_seconds();

	if (de_fbx_is_binary(file))
	{
		root = de_fbx_binary_load_file(file, &data_buf);
	}
	else
	{
		root = de_fbx_ascii_load_file(file, &data_buf);
	}

	if (!root)
	{
		de_log("FBX: Unable to load FBX from %s", file);
		return NULL;
	}

	de_log("FBX: %s parsed in %f seconds!", file, de_time_get_seconds() - last_time);

	fbx = de_fbx_read(root);

	if (!fbx)
	{
		de_fbx_node_free(root);
		return NULL;
	}

	de_fbx_node_free(root);
	root_node = de_fbx_to_scene(scene, fbx);
	de_fbx_free(fbx);

	de_log("FBX: %s is loaded in %f seconds!", file, de_time_get_seconds() - last_time);

	de_fbx_buffer_free(&data_buf);

	return root_node;
}

de_bool_t de_fbx_is_binary(const char* filename)
{
	char magic[18];
	de_bool_t result;

	FILE* file = fopen(filename, "rb");

	if (!file)
	{
		return DE_FALSE;
	}

	fread(magic, sizeof(char), 18, file);

	result = strncmp(magic, "Kaydara FBX Binary", 18) == 0;

	fclose(file);

	return result;
}