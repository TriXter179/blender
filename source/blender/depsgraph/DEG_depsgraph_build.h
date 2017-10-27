/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2013 Blender Foundation.
 * All rights reserved.
 *
 * Original Author: Joshua Leung
 * Contributor(s): Lukas Toenne
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file blender/depsgraph/DEG_depsgraph_build.h
 *  \ingroup depsgraph
 *
 * Public API for Depsgraph
 */

#ifndef __DEG_DEPSGRAPH_BUILD_H__
#define __DEG_DEPSGRAPH_BUILD_H__

/* ************************************************* */

/* Dependency Graph */
struct Depsgraph;

/* ------------------------------------------------ */

struct Main;
struct Scene;
struct Group;
struct EffectorWeights;
struct ModifierData;
struct Object;

#ifdef __cplusplus
extern "C" {
#endif

/* Graph Building -------------------------------- */

/* Build depsgraph for the given scene, and dump results in given
 * graph container.
 */
void DEG_graph_build_from_scene(struct Depsgraph *graph,
                                struct Main *bmain,
                                struct Scene *scene);

/* Tag relations from the given graph for update. */
void DEG_graph_tag_relations_update(struct Depsgraph *graph);

/* Tag all relations in the database for update.*/
void DEG_relations_tag_update(struct Main *bmain);

/* Create new graph if didn't exist yet,
 * or update relations if graph was tagged for update.
 */
void DEG_scene_relations_update(struct Main *bmain, struct Scene *scene);

/* Rebuild dependency graph only for a given scene. */
void DEG_scene_relations_rebuild(struct Main *bmain,
                                 struct Scene *scene);

/* Delete scene graph. */
void DEG_scene_graph_free(struct Scene *scene);

/* Add Dependencies  ----------------------------- */

/* Handle for components to define their dependencies from callbacks.
 * This is generated by the depsgraph and passed to dependency callbacks
 * as a symbolic reference to the current DepsNode.
 * All relations will be defined in reference to that node.
 */
struct DepsNodeHandle;

struct CacheFile;
struct Object;

typedef enum eDepsSceneComponentType {
	/* Parameters Component - Default when nothing else fits
	 * (i.e. just SDNA property setting).
	 */
	DEG_SCENE_COMP_PARAMETERS,
	/* Animation Component
	 * TODO(sergey): merge in with parameters?
	 */
	DEG_SCENE_COMP_ANIMATION,
	/* Sequencer Component (Scene Only). */
	DEG_SCENE_COMP_SEQUENCER,
} eDepsSceneComponentType;

typedef enum eDepsObjectComponentType {
	/* Parameters Component - Default when nothing else fits
	 * (i.e. just SDNA property setting).
	 */
	DEG_OB_COMP_PARAMETERS,
	/* Generic "Proxy-Inherit" Component.
	 * TODO(sergey): Also for instancing of subgraphs?
	 */
	DEG_OB_COMP_PROXY,
	/* Animation Component.
	 *
	 * TODO(sergey): merge in with parameters?
	 */
	DEG_OB_COMP_ANIMATION,
	/* Transform Component (Parenting/Constraints) */
	DEG_OB_COMP_TRANSFORM,
	/* Geometry Component (DerivedMesh/Displist) */
	DEG_OB_COMP_GEOMETRY,

	/* Evaluation-Related Outer Types (with Subdata) */

	/* Pose Component - Owner/Container of Bones Eval */
	DEG_OB_COMP_EVAL_POSE,
	/* Bone Component - Child/Subcomponent of Pose */
	DEG_OB_COMP_BONE,

	/* Particle Systems Component */
	DEG_OB_COMP_EVAL_PARTICLES,
	/* Material Shading Component */
	DEG_OB_COMP_SHADING,
	/* Cache Component */
	DEG_OB_COMP_CACHE,
} eDepsObjectComponentType;

void DEG_add_scene_relation(struct DepsNodeHandle *node,
                            struct Scene *scene,
                            eDepsSceneComponentType component,
                            const char *description);
void DEG_add_object_relation(struct DepsNodeHandle *node, struct
                             Object *ob,
                             eDepsObjectComponentType component,
                             const char *description);
void DEG_add_bone_relation(struct DepsNodeHandle *handle,
                           struct Object *ob,
                           const char *bone_name,
                           eDepsObjectComponentType component,
                           const char *description);
void DEG_add_object_cache_relation(struct DepsNodeHandle *handle,
                                   struct CacheFile *cache_file,
                                   eDepsObjectComponentType component,
                                   const char *description);


struct Depsgraph *DEG_get_graph_from_handle(struct DepsNodeHandle *handle);
void DEG_add_special_eval_flag(struct Depsgraph *graph, struct ID *id, short flag);

/* Utility functions for physics modifiers */
typedef bool (*DEG_CollobjFilterFunction)(struct Object *obj, struct ModifierData *md);

void DEG_add_collision_relations(struct DepsNodeHandle *handle,
                                 struct Scene *scene,
                                 struct Object *ob,
                                 struct Group *group,
                                 int layer,
                                 unsigned int modifier_type,
                                 DEG_CollobjFilterFunction fn,
                                 bool dupli,
                                 const char *name);
void DEG_add_forcefield_relations(struct DepsNodeHandle *handle,
                                  struct Scene *scene,
                                  struct Object *ob,
                                  struct EffectorWeights *eff,
                                  bool add_absorption,
                                  int skip_forcefield,
                                  const char *name);

/* ************************************************ */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* __DEG_DEPSGRAPH_BUILD_H__ */
