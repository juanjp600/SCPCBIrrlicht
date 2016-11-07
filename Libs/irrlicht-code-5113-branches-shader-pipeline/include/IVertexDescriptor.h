// Copyright (C) 2012 Patryk Nadrowski
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __I_VERTEX_DESCRIPTOR_H_INCLUDED__
#define __I_VERTEX_DESCRIPTOR_H_INCLUDED__

#include "IReferenceCounted.h"
#include "irrArray.h"
#include "irrString.h"

namespace irr
{
namespace video
{
	enum E_VERTEX_ATTRIBUTE_SEMANTIC
	{
		EVAS_POSITION = 0,
		EVAS_NORMAL,
		EVAS_COLOR,
		EVAS_TEXCOORD0,
		EVAS_TEXCOORD1,
		EVAS_TEXCOORD2,
		EVAS_TEXCOORD3,
		EVAS_TEXCOORD4,
		EVAS_TEXCOORD5,
		EVAS_TEXCOORD6,
		EVAS_TEXCOORD7,
		EVAS_TANGENT,
		EVAS_BINORMAL,
		EVAS_BLEND_WEIGHTS,
		EVAS_BLEND_INDICES,
		EVAS_CUSTOM,

		EVAS_COUNT
	};

	enum E_VERTEX_ATTRIBUTE_TYPE
	{
		EVAT_BYTE = 0,
		EVAT_UBYTE,
		EVAT_SHORT,
		EVAT_USHORT,
		EVAT_INT,
		EVAT_UINT,
		EVAT_FLOAT,
		EVAT_DOUBLE
	};

	enum E_INSTANCE_DATA_STEP_RATE
	{
		EIDSR_PER_VERTEX,
		EIDSR_PER_INSTANCE
	};

	class IVertexAttribute
	{
	public:
		virtual const core::stringc& getName() const = 0;

		virtual u32 getElementCount() const = 0;

		virtual E_VERTEX_ATTRIBUTE_SEMANTIC getSemantic() const = 0;

		virtual E_VERTEX_ATTRIBUTE_TYPE getType() const = 0;

		virtual u32 getTypeSize() const = 0;

		virtual u32 getOffset() const = 0;

		virtual u32 getBufferID() const = 0;
	};

	class IVertexDescriptor : public virtual IReferenceCounted
	{
	public:
		virtual u32 getID() const = 0;

		virtual const core::stringc& getName() const = 0;

		virtual u32 getVertexSize(u32 bufferID) const = 0;

		virtual E_INSTANCE_DATA_STEP_RATE getInstanceDataStepRate(u32 bufferID) const = 0;

		virtual void setInstanceDataStepRate(E_INSTANCE_DATA_STEP_RATE rate, u32 bufferID) = 0;

		virtual bool addAttribute(const core::stringc& name, u32 elementCount, E_VERTEX_ATTRIBUTE_SEMANTIC semantic, E_VERTEX_ATTRIBUTE_TYPE type, u32 bufferID) = 0;

		virtual IVertexAttribute* getAttribute(u32 id) const = 0;

		virtual IVertexAttribute* getAttributeByName(const core::stringc& name) const = 0;

		virtual IVertexAttribute* getAttributeBySemantic(E_VERTEX_ATTRIBUTE_SEMANTIC semantic) const = 0;

		virtual u32 getAttributeCount() const = 0;

		virtual bool removeAttribute(u32 id) = 0;

		virtual void removeAllAttribute() = 0;
	};
}
}

#endif
