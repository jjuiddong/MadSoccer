//------------------------------------------------------------------------
// Name:    DisplayObject.h
// Author:  jjuiddong
// Date:    2012-12-01
// 
// 화면에 표시되는 모든 것은 이 객체를 통한다.
//------------------------------------------------------------------------

#pragma once


namespace graphics
{
	class CDisplayObject : public CSyncNode
	{
	public:
		CDisplayObject(std::string name="DispObj");
		virtual ~CDisplayObject();

	protected:
		std::string		m_Name;
		Vector3			*m_pVertexBuffer;
		Short2			*m_pIndexBuffer;
		int				m_VertexBufferSize;
		int				m_IndexBufferSize;
		Matrix44		m_matTM;

	public:
		void			Load(Vector3 *pVtxBuff, int VtxSize, Short2 *pIdxBuff, int IdxSize );
		Vector3*		GetVertexBuffer() const { return m_pVertexBuffer; }
		Short2*			GetIndexBuffer() const { return m_pIndexBuffer; }
		int				GetVertexBufferSize() const { return m_VertexBufferSize; }
		int				GetIndexBufferSize() const { return m_IndexBufferSize; }

		void			Clear();

		// Setter/Getter


		// overriding
		virtual void	RenderGDI(HDC hdc);
		virtual void	Render();
		virtual void	Animation(int elapseTime); // milli second

	};

}
