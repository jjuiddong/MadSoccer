
#pragma once


namespace graphics
{
	class CDisplayObject
	{
	public:
		CDisplayObject();
		virtual ~CDisplayObject();

	protected:
		Vector3			*m_pVertexBuffer;
		Vector3			*m_pIndexBuffer;
		Matrix44		m_matTM;
		DispObjList		m_Child;

	public:
		void			Load(Vector3 *pVtxBuff, int VtxSize, Vector3 *pIdxBuff, int IdxSize );
		Vector3*		GetVertexBuffer() const { return m_pVertexBuffer; }
		Vector3*		GetIndexBuffer() const { return m_pIndexBuffer; }

		bool			AddChild(CDisplayObject *pChild);
		bool			RemoveChild(CDisplayObject *pChild);
		void			Clear();

		// overriding
		virtual void	Render();
		virtual void	Animation(int elapseTime); // milli second

	};

}
