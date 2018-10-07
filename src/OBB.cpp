#include "OBB.h"
#include "math.hpp"


	OBB::OBB()
	{

	}

	OBB::~OBB()
	{}

	void OBB::Create(VertexBufferType & t_ModelVertexList, vec3& newCenter)
	{
		
	}






	void OBB::UpdateOBB(const mat4& transform)
	{
		//update center
		vec4 position(this->m_center, 1);
		m_center = vec3(transform * position);
		mat3 scaleMat(Scale(m_radius));
		mat3 temp;

		//calculate as mat 3 to same some computations
		temp = mat3(transform) *m_localAxes* scaleMat;

		//extract radius in world space
		m_radius.x = glm::length(temp[0]);
		m_radius.y = glm::length(temp[1]);
		m_radius.z = glm::length(temp[2]);

		//extract axis in world space
		m_localAxes[0] = Normalise(temp[0]);
		m_localAxes[1] = Normalise(temp[1]);
		m_localAxes[2] = Normalise(temp[2]);


	}


	mat4& OBB::GetBoxTransform(mat4& output)
	{
		output[0] = vec4((this->m_localAxes[0] * this->m_radius[0] * 2.f), 0);
		output[1] = vec4((this->m_localAxes[1] * this->m_radius[1] * 2.f), 0);
		output[2] = vec4((this->m_localAxes[2] * this->m_radius[2] * 2.f), 0);
		//output = Transpose(output);
		output[3] = vec4((this->m_center), 1);

		return output;
	}





	// builds an OBB from triangles specified as an array of
	// points with integer indices into the point array. Forms
	// the covariance matrix for the triangles, then uses the
	// method build_from_covariance_matrix() method to fit 
	// the box.  ALL points will be fit in the box, regardless
	// of whether they are indexed by a triangle or not.
	void OBB::BuildFromModel(const VertexBufferType &pnts, const std::vector<int> &indicies){

		f32 Ai, Am = 0.0;
		vec3 mu(0.0f, 0.0f, 0.0f), bc;
		mat3 C;
		f32 cxx = 0.0, cxy = 0.0, cxz = 0.0, cyy = 0.0, cyz = 0.0, czz = 0.0;
#if 1
		// loop over the triangles this time to find the
		// mean location
		int totalTris = indicies.size();
		for (int i = 0; i<totalTris; i += 3){
			const Vec3 &p = pnts[indicies[i	 ]].pos;		
			const Vec3 &q = pnts[indicies[i + 1]].pos;
			const Vec3 &r = pnts[indicies[i + 2]].pos;//get vertex of triangles
			bc = (p + q + r) / 3.0f; //baricenter of the points p + q + r
			vec3 dir = glm::cross((q - p), (r - p));
			Ai = glm::length(dir) * 0.5f; // area of the triangle
			mu += bc*Ai; // this is for?
			Am += Ai; //accumulate the surface area of the model
			f32 aiFrac = (Ai / 12.0f); // why divide by 12?
			// these bits set the c terms to Am*E[xx], Am*E[xy], Am*E[xz]....
			cxx += ((9.0f*bc.x*bc.x) + (p.x*p.x) + (q.x*q.x) + (r.x*r.x))* aiFrac;
			cxy += ((9.0f*bc.x*bc.y) + (p.x*p.y) + (q.x*q.y) + (r.x*r.y))* aiFrac;
			cxz += ((9.0f*bc.x*bc.z) + (p.x*p.z) + (q.x*q.z) + (r.x*r.z))* aiFrac;
			cyy += ((9.0f*bc.y*bc.y) + (p.y*p.y) + (q.y*q.y) + (r.y*r.y))* aiFrac;
			cyz += ((9.0f*bc.y*bc.z) + (p.y*p.z) + (q.y*q.z) + (r.y*r.z))* aiFrac; //covariance term accumulation
			
		}
		// divide out the Am fraction from the average position and 
		// covariance terms
		if (Am == 0.f) Am = 1;
		mu /= Am; //find mean values  E[xx], E[yy], E[zz]
		cxx /= Am; 
		cxy /= Am; 
		cxz /= Am; 
		cyy /= Am; 
		cyz /= Am; 
		czz /= Am; //average based on surface area

		cxx -= mu.x*mu.x; 
		cxy -= mu.x*mu.y; 
		cxz -= mu.x*mu.z; 
		cyy -= mu.y*mu.y; 
		cyz -= mu.y*mu.z; 
		czz -= mu.z*mu.z; // subtract mean terms

		// now build the covariance matrix
		C[0][0] = cxx; C[0][1] = cxy; C[0][2] = cxz;
		C[1][0] = cxy; C[1][1] = cyy; C[1][2] = cyz;
		C[2][0] = cxz; C[1][2] = cyz; C[2][2] = czz;

		// set the obb parameters from the covariance matrix
		CovarianceMatrix(C, pnts, indicies);
#endif
	}

	// this code is for example purposes only and is likely to be inefficient.
	// it simply builds the convex hull using CGAL, then tesselates the output
	// of the convex hull and passes it to the build_from_triangles() method
	// above.  


	void OBB::CovarianceMatrix(mat3 &C, const VertexBufferType &pnts, const std::vector<int> &indicies){
		// extract the eigenvalues and eigenvectors from C
		mat3 eigvec;
		std::vector<f32> eigval(3);
#if 0

#else
		//gmm::symmetric_qr_algorithm(C, eigval, eigvec); //read up on qr factorization
#if 1
		modified_gram_schmidt(eigvec, C);

		// find the right, up and forward vectors from the eigenvectors
		Vec3 r(eigvec[0][0], eigvec[0][1], eigvec[0][2]);
		Vec3 u(eigvec[1][0], eigvec[1][1], eigvec[1][2]);
		Vec3 f(eigvec[2][0], eigvec[2][1], eigvec[2][2]);
		//r = Normalize(r);
		//u = Normalize(u);
		//f = Normalize(f);
#else
		std::array<f64, 3> eval;
		std::array<std::array<f64, 3>, 3> evec;
		NISymmetricEigensolver3x3<f64> solver;
		solver(
			C[0][0], C[1][0], C[2][0],
			C[1][1], C[2][1], C[2][2],
			eval,
			evec
			);

		// find the right, up and forward vectors from the eigenvectors
#if 1
		Vec3 r(evec[0][0], evec[1][0], evec[2][0]);
		Vec3 u(evec[0][1], evec[1][1], evec[2][1]);
		Vec3 f(evec[0][2], evec[1][2], evec[2][2]);
#else
		Vec3 r(evec[0][0], evec[0][1], evec[0][2]);
		Vec3 u(evec[1][0], evec[1][1], evec[1][2]);
		Vec3 f(evec[2][0], evec[2][1], evec[2][2]);
#endif
		//r = Normalize(r);
		//u = Normalize(u);
		//f = Normalize(f);
#endif



		// set the rotation matrix using the eigvenvectors
#if 0
		m_localAxes[0][0] = r.x; m_localAxes[0][1] = u.x; m_localAxes[0][2] = f.x;
		m_localAxes[1][0] = r.y; m_localAxes[1][1] = u.y; m_localAxes[1][2] = f.y;
		m_localAxes[2][0] = r.z; m_localAxes[2][1] = u.z; m_localAxes[2][2] = f.z;
#else
		m_localAxes[0][0] = r.x; m_localAxes[1][0] = u.x; m_localAxes[2][0] = f.x;
		m_localAxes[0][1] = r.y; m_localAxes[1][1] = u.y; m_localAxes[2][1] = f.y;
		m_localAxes[0][2] = r.z; m_localAxes[1][2] = u.z; m_localAxes[2][2] = f.z;
#endif
		// now build the bounding box extents in the rotated frame
		Vec3 minim(1e10, 1e10, 1e10), maxim(-1e10, -1e10, -1e10);
		u32 total = indicies.size();
		for (u32 i = 0; i<total; i++){
			u32 index = indicies[i];
			Vec3 p_prime(Dot(r, pnts[index].pos), Dot(u, pnts[index].pos), Dot(f, pnts[index].pos));
			minim.x = std::min(p_prime.x, minim.x);
			minim.y = std::min(p_prime.y, minim.y);
			minim.z = std::min(p_prime.z, minim.z);
			maxim.x = std::max(p_prime.x, maxim.x);
			maxim.y = std::max(p_prime.y, maxim.y);
			maxim.z = std::max(p_prime.z, maxim.z);

		}

		// set the center of the OBB to be the average of the 
		// minimum and maximum, and the extents be half of the
		// difference between the minimum and maximum
		Vec3 center = (maxim + minim)*0.5f;
		mat3 transpose;
		//transpose = m_localAxes;
		transpose = glm::transpose(m_localAxes);
		m_center = vec3(Dot(transpose[0], center), Dot(transpose[1], center), Dot(transpose[2], center));
		m_radius = (maxim - minim)*0.5f;


#endif
	}


