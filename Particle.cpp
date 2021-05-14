struct particle {
	float x0, y0, z0;	// starting location	
	float vx0, vy0, vz0;	// starting velocity		
	float r0, g0, b0;	// starting color		
	float t0, t1;		// birth time, death time	
	float x, y, z;		// current location	
	float vx, vy, vz;	// current velocity		
	float r, g, b;		// current color
	float radius;
	float size;
	float xRot, yRot, zRot, rotAngle, rotSpeed, rotDir;
	float rockType;
};