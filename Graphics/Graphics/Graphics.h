
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


namespace graphics
{
	void Init(HWND hWnd);
	void Draw();


	// render
	void Render( Vector3 *pVtxBuff, Vector3 *pIdBuff );


};

#endif __GRAPHICS_H__
