//-----------------------------------------------------------------------------
// Walkabout Navigation Toolkit
// Copyright (c) 2012 Daniel Buckmaster
// Permission is NOT granted to freely distribute the contents of this file.
//-----------------------------------------------------------------------------

#ifndef _NAVPATH_H_
#define _NAVPATH_H_

#include "scene/sceneObject.h"
#include "scene/simPath.h"
#include "navMesh.h"
#include "recast/DetourNavMeshQuery.h"

class NavPath: public SceneObject {
   typedef SceneObject Parent;
   static const U32 MaxPathLen = 2048;
public:
   /// @name NavPath
   /// Functions for planning and accessing the path.
   /// @{

   String mMeshName;
   NavMesh *mMesh;
   SimPath::Path *mWaypoints;

   Point3F mFrom;
   bool mFromSet;
   Point3F mTo;
   bool mToSet;

   bool mIsLooping;
   bool mAutoUpdate;
   bool mIsSliced;

   S32 mMaxIterations;

   bool mAlwaysRender;
   bool mXray;
   bool mRenderSearch;

   /// What sort of link types are we allowed to move on?
   LinkData mLinkTypes;

   /// Plan the path.
   bool plan();

   /// Updated a sliced plan.
   /// @return True if we need to keep updating, false if we can stop.
   bool update();

   /// Finalise a sliced plan.
   /// @return True if the plan was successful overall.
   bool finalise();

   /// Did the path plan successfully?
   bool success() const { return dtStatusSucceed(mStatus); }

   /// @}

   /// @name Path interface
   /// These functions are provided to make NavPath behave
   /// similarly to the existing Path class, despite NavPath
   /// not being a SimSet.
   /// @{

   /// Return the length of this path.
   F32 getLength() const { return mLength; };

   /// Get the number of nodes in a path.
   S32 size() const;

   /// Return world-space position of a path node.
   Point3F getNode(S32 idx) const;

   /// Get the flags for a given path node.
   U16 getFlags(S32 idx) const;

   /// @}

   /// @name SceneObject
   /// @{

   static void initPersistFields();

   bool onAdd();
   void onRemove();

   void onEditorEnable();
   void onEditorDisable();
   void inspectPostApply();

   void onDeleteNotify(SimObject *object);

   U32 packUpdate(NetConnection *conn, U32 mask, BitStream *stream);
   void unpackUpdate(NetConnection *conn, BitStream *stream);

   void prepRenderImage(SceneRenderState *state);
   void renderSimple(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance *overrideMat);

   DECLARE_CONOBJECT(NavPath);

   /// @}

   /// @name ProcessObject
   /// @{
   void processTick(const Move *move);
   /// @}

   NavPath();
   ~NavPath();

protected:
   enum masks {
      PathMask     = Parent::NextFreeMask << 0,
      NextFreeMask = Parent::NextFreeMask << 1
   };

private:
   /// Create appropriate data structures and stuff.
   bool init();

   /// Plan the path.
   bool planInstant();

   /// Start a sliced plan.
   /// @return True if the plan initialised successfully.
   bool planSliced();

   /// Add points of the path between the two specified points.
   //bool addPoints(Point3F from, Point3F to, Vector<Point3F> *points);

   /// 'Visit' the last two points on our visit list.
   bool visitNext();

   dtNavMeshQuery *mQuery;
   dtStatus mStatus;
   dtQueryFilter mFilter;
   S32 mCurIndex;
   Vector<Point3F> mPoints;
   Vector<unsigned short> mFlags;
   Vector<Point3F> mVisitPoints;
   F32 mLength;

   /// Resets our world transform and bounds to fit our point list.
   void resize();

   /// Function used to set mMesh object from console.
   static bool setProtectedMesh(void *obj, const char *index, const char *data);

   /// Function used to set mWaypoints from console.
   static bool setProtectedWaypoints(void *obj, const char *index, const char *data);

   void checkAutoUpdate();
   /// Function used to protect auto-update flag.
   static bool setProtectedAutoUpdate(void *obj, const char *index, const char *data);

   /// @name Protected from and to vectors
   /// @{
   static bool setProtectedFrom(void *obj, const char *index, const char *data);
   static bool setProtectedTo(void *obj, const char *index, const char *data);
   static const char *getProtectedFrom(void *obj, const char *data);
   static const char *getProtectedTo(void *obj, const char *data);
   /// @}
};

#endif
