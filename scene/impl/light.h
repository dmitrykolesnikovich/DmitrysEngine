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

/*=======================================================================================*/
void de_light_init(de_light_t* light)
{
	de_color_set(&light->color, 255, 255, 255, 255);
	light->radius = 2.0f;
	light->type = DE_LIGHT_TYPE_POINT;	
	light->cone_angle = M_PI;
	light->cone_angle_cos = -1.0f;
}

/*=======================================================================================*/
void de_light_deinit(de_light_t* light)
{
	DE_UNUSED(light);
}

/*=======================================================================================*/
void de_light_set_radius(de_node_t * node, float radius)
{
	DE_ASSERT_SCENE_NODE_TYPE(node, DE_NODE_TYPE_LIGHT);
	node->s.light.radius = de_maxf(FLT_EPSILON, radius);
}

/*=======================================================================================*/
void de_light_set_cone_angle(de_node_t* node, float angle)
{
	DE_ASSERT_SCENE_NODE_TYPE(node, DE_NODE_TYPE_LIGHT);
	de_light_t* light = &node->s.light;
	light->cone_angle = angle;
	light->cone_angle_cos = cos(angle);
}

/*=======================================================================================*/
float de_light_get_cone_angle(de_node_t* node)
{
	DE_ASSERT_SCENE_NODE_TYPE(node, DE_NODE_TYPE_LIGHT);
	return node->s.light.cone_angle;
}