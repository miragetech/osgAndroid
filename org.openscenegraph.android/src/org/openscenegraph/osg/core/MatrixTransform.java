/* @License 
 -------------------------------------------------------------------------------
 | osgAndroid - Copyright (C) 2012 Rafael Gait‡n, Mirage Technologies S.L.     |
 |                                                                             |
 | This library is free software; you can redistribute it and/or modify        |
 | it under the terms of the GNU Lesser General Public License as published    |
 | by the Free Software Foundation; either version 3 of the License, or        |
 | (at your option) any later version.                                         |
 |                                                                             |
 | This library is distributed in the hope that it will be useful, but         |
 | WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY  |
 | or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public     |
 | License for more details.                                                   |
 |                                                                             |
 | You should have received a copy of the GNU Lesser General Public License    |
 | along with this software; if not, write to the Free Software Foundation,    |
 | Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.               |
 ---------------------------------------------------------------------------- */
package org.openscenegraph.osg.core;

import org.openscenegraph.osg.Library;

public class MatrixTransform extends Group {
	static {
		Library.initLibrary();
	}

	private static native long nativeCreateMatrixTransform();

	private static native void nativeDispose(long cptr);

	private static native void nativeSetMatrix(long cptr, long matrix);

	private static native long nativeGetMatrix(long cptr);

	public MatrixTransform(long cptr) {
		_cptr = cptr;
	}

	@Override
	protected void finalize() throws Throwable {
		dispose();
		super.finalize();
	}

	public void dispose() {
		if (_cptr != 0)
			nativeDispose(_cptr);
		_cptr = 0;
	}

	public MatrixTransform() {
		_cptr = nativeCreateMatrixTransform();
	}

	public void setMatrix(Matrix matrix) {
		nativeSetMatrix(_cptr, matrix.getNativePtr());
	}

	public Matrix getMatrix() {
		Matrix mat = new Matrix(nativeGetMatrix(_cptr));
		return mat;
	}
}
