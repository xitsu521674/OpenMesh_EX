#include "GUA_OM.h"

namespace OMT
{
	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
	}
}
/*======================================================================*/
namespace OMP
{
	Model::Model()
	{
		Mesh.request_vertex_status();
		Mesh.request_edge_status();
		Mesh.request_face_status();
	}
	Model::~Model()
	{
		Mesh.release_vertex_status();
		Mesh.release_edge_status();
		Mesh.release_face_status();
	}
	/*======================================================================*/
	bool Model::ReadFile(std::string _fileName)
	{
		bool isRead = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::read_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isRead = true;
		}
		if(isRead)
		{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
			if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && Mesh.has_vertex_normals())
			{
				Mesh.update_normals();
			}
		}
		return isRead;
	}
	bool Model::SaveFile(std::string _fileName)
	{
		bool isSave = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::write_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isSave = true;
		}
		return isSave;
	}
	/*======================================================================*/
	void Model::Render_solid()
	{
		FIter f_it;
		FVIter	fv_it;
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.5, 1.0, 0.5);
		for (f_it = Mesh.faces_begin(); f_it != Mesh.faces_end(); ++f_it) 
		{
			for (fv_it = Mesh.fv_iter( f_it ); fv_it; ++fv_it)
			{						
				glNormal3dv(Mesh.normal(fv_it.handle()).data());
				glVertex3dv(Mesh.point(fv_it.handle()).data());
			}
		}
		glEnd();		
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::Render_wireframe()
	{
		MyMesh::HalfedgeHandle _hedge;
		EIter e_it=Mesh.edges_begin();

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for(e_it=Mesh.edges_begin(); e_it != Mesh.edges_end(); ++e_it)
		{
			_hedge = Mesh.halfedge_handle(e_it.handle(),1);

			glVertex3dv(Mesh.point(Mesh.from_vertex_handle(_hedge)).data());
			glVertex3dv(Mesh.point(Mesh.to_vertex_handle(_hedge)).data());			
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	/*======================================================================*/
	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector<sp_p>::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(p_itr->pt.data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector< sp_v >::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(Mesh.point(v_itr->vh).data());
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_itr;
		vector< sp_f >::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr=Mesh.fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{						
				glNormal3dv(Mesh.normal(fv_itr.handle()).data());
				glVertex3dv(Mesh.point(fv_itr.handle()).data());
			}
		}
		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	/*======================================================================*/
	void Model::add_sp_p(Point   _p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}
	void Model::add_sp_v(VHandle _v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}
	void Model::add_sp_f(FHandle _f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}
	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}
	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}
	/*======================================================================*/
	VHandle Model::addVertex(Point _p)
	{
		int find_result = findVertex(_p);
		if (find_result != -1)
		{
			return Mesh.vertex_handle(find_result);
		}
		else
		{
			return Mesh.add_vertex(_p);
		}
	}
	FHandle Model::addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		vector<VHandle> face_vhandles;

		face_vhandles.clear();
		face_vhandles.push_back(_v0);
		face_vhandles.push_back(_v1);
		face_vhandles.push_back(_v2);
		face_vhandles.push_back(_v3);
		return Mesh.add_face(face_vhandles);
	}
	void Model::deleteFace(FHandle _f)
	{
		Mesh.delete_face(_f);
		Mesh.garbage_collection();
	}
	void Model::deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		/* 
		v1				v0
		*<--------------*
		|				|
		|				|
		|				|
		|		f		|
		|				|
		|				|
		|				|
		* --------------*
		v2				v3
		*/

		HEHandle v0v1 = Mesh.find_halfedge(_v0, _v1);
		if (v0v1.is_valid())
		{
			FHandle fh = Mesh.face_handle(v0v1);
			if (fh.is_valid())
			{
				Mesh.delete_face(fh);
				Mesh.garbage_collection();
			}
		}
	}
	/*======================================================================*/
	bool Model::IsVertexVertex( VHandle _vj, VHandle _vi)
	{
		for( VVIter vvit = Mesh.vv_iter(_vi) ; vvit ; ++vvit )
			if( vvit.handle() == _vj )
				return true;
		return false;
	}
	/*======================================================================*/
	int Model::quad_subdivision1(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vd		v3
			*-------*-------*
			|		|		|	 
			|		|		|
			|	  ve|		|
		  va*-------*-------*vc
			|		|		|
			|		|		|
			|		|		|
			*-------*-------*
			v1		vb		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1]))/2);
			vb	 = addVertex((Mesh.point(v[1])+Mesh.point(v[2]))/2);
			vc	 = addVertex((Mesh.point(v[2])+Mesh.point(v[3]))/2);
			vd	 = addVertex((Mesh.point(v[3])+Mesh.point(v[0]))/2);
			ve	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1])+Mesh.point(v[2])+Mesh.point(v[3]))/4);
			addFace(vd, v[0], va, ve);
			addFace(va, v[1], vb, ve);
			addFace(vb, v[2], vc, ve);
			addFace(vc, v[3], vd, ve);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vh		vg		v3
			*-------*-------*-------*
			|		|		|		|
			|		|		|		|
			|	  vi|	  vl|		|
		 va *-------*-------*-------*vf
			|		|		|		|
			|		|		|		|
			|	  vj|	  vk|		|
		 vb *-------*-------*-------*ve
			|		|		|		|
			|		|		|		|
			|		|		|		|
			*-------*-------*-------*
			v1		vc		vd		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])*2+Mesh.point(v[1])  )/3);
			vb	 = addVertex((Mesh.point(v[0])  +Mesh.point(v[1])*2)/3);
			vc	 = addVertex((Mesh.point(v[1])*2+Mesh.point(v[2])  )/3);
			vd	 = addVertex((Mesh.point(v[1])  +Mesh.point(v[2])*2)/3);
			ve	 = addVertex((Mesh.point(v[2])*2+Mesh.point(v[3])  )/3);
			vf	 = addVertex((Mesh.point(v[2])  +Mesh.point(v[3])*2)/3);
			vg	 = addVertex((Mesh.point(v[3])*2+Mesh.point(v[0])  )/3);
			vh	 = addVertex((Mesh.point(v[3])  +Mesh.point(v[0])*2)/3);

			vi	 = addVertex((Mesh.point(vh)*2+Mesh.point(vc)  )/3);
			vj	 = addVertex((Mesh.point(vh)  +Mesh.point(vc)*2)/3);
			vk	 = addVertex((Mesh.point(vd)*2+Mesh.point(vg)  )/3);
			vl	 = addVertex((Mesh.point(vd)  +Mesh.point(vg)*2)/3);
			addFace(v[0], va, vi, vh);
			addFace(va, vb, vj, vi);
			addFace(vb, v[1], vc, vj);
			addFace(vc, vd, vk, vj);
			addFace(vd, v[2], ve, vk);
			addFace(ve, vf, vl, vk);
			addFace(vf, v[3], vg, vl);
			addFace(vg, vh, vi, vl);
			addFace(vi, vj, vk, vl);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	/*======================================================================*/
	int Model::findVertex(Point _p)
	{
		for( VIter v_itr = Mesh.vertices_begin(); v_itr != Mesh.vertices_end(); ++v_itr)
			if( Mesh.point(v_itr) == _p )
				return v_itr.handle().idx();
		return -1;
	}
	/*======================================================================*/
};
/*======================================================================*/
void Tri_Mesh::Render_Solid()
{
		FIter f_it;
		FVIter	fv_it;
		//glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LIGHTING);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_TRIANGLES);
		glColor4f(0.81, 0.74, 0.33, 0.3);
		for (f_it = faces_begin(); f_it != faces_end(); ++f_it) 
		{
			for (fv_it = fv_iter( f_it ); fv_it; ++fv_it)
			{						
				glNormal3dv(normal(fv_it.handle()).data());
				glVertex3dv(point(fv_it.handle()).data());
			}
		}
		glEnd();	
		
		glDisable(GL_POLYGON_OFFSET_FILL);
}

void Tri_Mesh::Render_SolidWireframe()
{
	FIter f_it;
	FVIter	fv_it;

	/*glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3dv(point(vertex_handle(0)));
	glEnd();*/
    glDisable(GL_LIGHTING);
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 0.96, 0.49, 1.0);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) 
	{
		for (fv_it = fv_iter( f_it ); fv_it; ++fv_it)
		{						
			//glNormal3dv(normal(fv_it.handle()));
			glVertex3dv(point(fv_it.handle()).data());
		}
	}
	glEnd();

	
	//glDisable(GL_POLYGON_OFFSET_FILL);


	glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);	
	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());			
	}
	glEnd();
	glPopAttrib();
}
void Tri_Mesh::initAvailableData() {
	FIter f_it;
	FVIter	fv_it;
	vertexNumber = 0;
	faceNumber = 0;
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) {
		faceNumber++;
	}
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it)
	{
		for (fv_it = fv_iter(f_it); fv_it; ++fv_it)
		{
			vertexData.push_back(point(fv_it.handle())[0]);
			vertexData.push_back(point(fv_it.handle())[1]);
			vertexData.push_back(point(fv_it.handle())[2]);
			faceData.push_back((f_it->idx()));
			//faceData.push_back(1);
			//std::cout << ((float)(f_it->idx()) / (float)faceNumber) << "\n";
			vertexNumber++;
		}
	}
}
void Tri_Mesh::generateChosenData() {
	FIter f_it;
	FVIter	fv_it;
	isChosenFaceGathing = true;
	//取得所有選取面的點
	chosenFaceData.clear();
	std::set<VertexHandle> chosenVertices;
	for (int offset : chosenFace)
	{
		FaceHandle _fh = face_handle(offset);
		for (fv_it = fv_iter(_fh); fv_it; ++fv_it)
		{
			VertexHandle _vh = fv_it.handle();
			chosenVertices.insert(_vh);
		}
		for (int i = 0; i < 9; ++i) {
			chosenFaceData.push_back(vertexData[offset * 9 + i]);
		}
	}
	//將選取面的點分為外部與內部
	outsideVertex.clear();
	insideVertex.clear();
	
	std::map<std::pair<int,int>,int> edgeCount;
	for (int offset : chosenFace)
	{
		FaceHandle _fh = face_handle(offset);
		std::vector<int> vtx;
		for (fv_it = fv_iter(_fh); fv_it; ++fv_it)
		{
			VertexHandle _vh = fv_it.handle();
			vtx.push_back(_vh.idx());
		}
		for (int i = 0; i < vtx.size(); ++i) {
			for (int j = i + 1; j < vtx.size(); ++j) {
				int v1 = vtx[i];
				int v2 = vtx[j];
				std::pair<int, int> p;
				p.first = v1 < v2 ? v1 : v2;
				p.second = v1 > v2 ? v1 : v2;
				edgeCount[p]++;
			}
		}
	}
	std::cout << "edge number:" << edgeCount.size() << "\n";
	std::map<int, int> outsideVerticesAppearTimes;
	for (std::pair<std::pair<int,int>, int> p : edgeCount) {
		if (p.second == 1) {
			outsideVertex.insert(p.first.first);
			outsideVertex.insert(p.first.second);
			outsideVerticesAppearTimes[p.first.first]++;
			outsideVerticesAppearTimes[p.first.second]++;
		}
	}
	std::set<std::pair<int, int>> outsideEdges;
	

	for (std::pair<std::pair<int, int>, int> p : edgeCount) {
		if (p.second != 1) {
			if (outsideVertex.find(p.first.first) == outsideVertex.end()) {
				insideVertex.insert(vertex_handle(p.first.first));
			}
			if (outsideVertex.find(p.first.second) == outsideVertex.end()) {
				insideVertex.insert(vertex_handle(p.first.second));
			}
		}
		//篩選出外部edge
		else if (p.second == 1) {
			int v1 = p.first.first;
			int v2 = p.first.second;
			if (outsideVerticesAppearTimes[v1] == 2 && outsideVerticesAppearTimes[v2] == 2) {
				outsideEdges.insert(p.first);
			}
		}
	}
	//將外部點排成連續
	orderedOutsideVertex.clear();
	std::set<int> usedVertex;
	int currentOrdering = *(outsideVertex.begin());
	orderedOutsideVertex.push_back(vertex_handle(currentOrdering));
	usedVertex.insert(currentOrdering);
	int record = usedVertex.size();
	while (usedVertex.size()!=outsideVertex.size()) {
		record = usedVertex.size();
		VVIter vv_it;
		/*std::cout << "find vertex connect to " << currentOrdering << "\n";
		std::cout << "remain candidate:";
		for (int val : outsideVertex) {
			if (usedVertex.find(val) == usedVertex.end()) {
				std::cout << val << " ";
			}
		}
		std::cout << "\n";*/
		for (auto iter = outsideEdges.begin(); iter != outsideEdges.end(); ++iter) {
			int v1 = (*iter).first;
			int v2 = (*iter).second;
			//std::cout << "try edge " << v1 << " " << v2 << "\n";
			if (v1 == currentOrdering) {
				currentOrdering = v2;
				usedVertex.insert(currentOrdering);
				orderedOutsideVertex.push_back(vertex_handle(currentOrdering));
				outsideEdges.erase(iter);
				break;
			}
			else if (v2 == currentOrdering) {
				currentOrdering = v1;
				usedVertex.insert(currentOrdering);
				orderedOutsideVertex.push_back(vertex_handle(currentOrdering));
				outsideEdges.erase(iter);
				break;
			}
		}
		if (record == usedVertex.size()) {
			isChosenFaceGathing = false;
			break;
		}
	}
	std::cout << "Outsidevertexs:" << outsideVertex.size() << "\n";
	std::cout << "orderedOutsidevertexs:" << orderedOutsideVertex.size() << "\n";
	std::cout << "Is grouping:" << isChosenFaceGathing << "\n";

	//
	outsideVertexData.clear();
	for (int idx : outsideVertex) {
		Point p = point(vertex_handle(idx));
		outsideVertexData.push_back(p[0]);
		outsideVertexData.push_back(p[1]);
		outsideVertexData.push_back(p[2]);
	}
}
void Tri_Mesh::computeTextureCoordinate() {
	if (!isChosenFaceGathing) {
		return;
	}
	OpenMesh::VPropHandleT<OpenMesh::Vec2d> newPos;
	OpenMesh::HPropHandleT<double> weight;
	add_property(newPos, "newPos");
	add_property(weight, "weight");

	textureData.clear();
	double modelLength = 0;
	for (int i = 0; i < orderedOutsideVertex.size(); ++i) {
		if (i == orderedOutsideVertex.size() - 1) {
			Point p1 = point(orderedOutsideVertex[0]);
			Point p2 = point(orderedOutsideVertex[i]);
			modelLength += (p1 - p2).norm();
		}
		else {
			Point p1 = point(orderedOutsideVertex[i + 1]);
			Point p2 = point(orderedOutsideVertex[i]);
			modelLength += (p1 - p2).norm();
		}
	}
	Point pos;
	pos[0] = 0;
	pos[1] = 0;
	property(newPos, orderedOutsideVertex[0]) = OpenMesh::Vec2d(pos[0], pos[1]);
	double accLength = 0.0f;
	double oneSideLength = modelLength / 4.0;
	for (int i = 1; i < orderedOutsideVertex.size(); ++i){
		Point p1 = point(orderedOutsideVertex[i]);
		Point p2 = point(orderedOutsideVertex[i - 1]);
		float currentLength = (p1 - p2).norm();
		accLength += currentLength;
		switch ((int)(accLength/oneSideLength))
		{
		case 0:
			pos[0] = accLength;
			pos[1] = 0;
			break;
		case 1:
			pos[0] = oneSideLength;
			pos[1] = accLength-oneSideLength;
			break;
		case 2:
			pos[0] = 3.0*oneSideLength-accLength;
			pos[1] = oneSideLength;
			break;
		case 3:
			pos[0] = 0;
			pos[1] = 4.0*oneSideLength-accLength;
			break;
		default:
			break;
		}
		pos[0] /= oneSideLength;
		pos[1] /= oneSideLength;
		property(newPos, orderedOutsideVertex[i]) = OpenMesh::Vec2d(pos[0], pos[1]);
	}
	//開始計算各half edge 的weight
	for (VertexHandle _vh : insideVertex) {
		VOHIter voh_it;
		std::vector<HHandle> HHs;
		for (voh_it = voh_iter(_vh); voh_it; ++voh_it) {
			HHs.push_back(voh_it.handle());
		}
		double totalWeight = 0.0;
		for (int i = 0; i < HHs.size(); ++i) {
			HHandle lasthh, curhh, nxthh;
			if (i == 0) {
				lasthh = HHs[HHs.size() - 1];
				curhh = HHs[0];
				nxthh = HHs[1];
			}
			else if(i==HHs.size() - 1)
			{
				lasthh = HHs[i - 1];
				curhh = HHs[i];
				nxthh = HHs[0];
			}
			else
			{
				lasthh = HHs[i - 1];
				curhh = HHs[i];
				nxthh = HHs[i + 1];
			}
			Point ori = point(from_vertex_handle(curhh));
			Point p1 = point(to_vertex_handle(lasthh));
			Point p2 = point(to_vertex_handle(curhh));
			Point p3 = point(to_vertex_handle(nxthh));
			double angBeta,angGamma;
			Point edge1 = ori - p1;
			edge1.normalize();
			Point edge2 = p2 - p1;
			edge2.normalize();
			double dot1 = OpenMesh::dot(edge1, edge2);
			if (dot1 >= 1.0) {
				angBeta = 0;
			}
			else if (dot1 <= -1.0) {
				angBeta = 3.14159265358979323846;
			}
			else {
				angBeta = std::acos(dot1);
			}
			
			

			Point edge3 = p2 - p3;
			edge3.normalize();
			Point edge4 = ori - p3;
			edge4.normalize();
			double dot2 = OpenMesh::dot(edge3, edge4);
			if (dot2 >= 1.0) {
				angGamma = 0;
			}
			else if (dot2 <= -1.0) {
				angGamma = 3.14159265358979323846;
			}
			else {
				angGamma = std::acos(dot2);
				if (isnan(angGamma))
					std::cout << "the dot cause nan is " << dot2 << "\n";
			}
			
			property(weight, HHs[i]) =( std::cos(angBeta) / std::sin(angBeta) + std::cos(angGamma) / std::sin(angGamma));
			totalWeight += property(weight, HHs[i]);
			//property(weight, HHs[i]) = 1;
			//std::cout << "ANGLE:" << angBeta << " " << angGamma << "\n";
			//std::cout << "cos/sin:" << std::cos(angBeta) << "/" << std::sin(angBeta) << ":" << std::cos(angGamma) << "/" << std::sin(angGamma) << "\n";
			std::cout << "weight:" << property(weight, HHs[i]) << "\n";
		}
		for (int i = 0; i < HHs.size(); ++i) {
			property(weight, HHs[i]) = property(weight, HHs[i]) / totalWeight;
		}

	}
	orderedInsideVertex.clear();
	for (VHandle _vh : insideVertex) {
		orderedInsideVertex.push_back(_vh);
	}
	std::cout << "insidevertex:" << orderedInsideVertex.size() << "\n";
	//計算線性系統
	if (orderedInsideVertex.size() > 0) {
		Eigen::SparseMatrix<double> A(orderedInsideVertex.size(), orderedInsideVertex.size());
		std::vector<Eigen::VectorXd> B;
		B.resize(2);
		B[0].resize(orderedInsideVertex.size());
		B[1].resize(orderedInsideVertex.size());
		B[0].setZero();
		B[1].setZero();

		for (int i = 0; i < orderedInsideVertex.size(); ++i) {
			A.insert(i, i) = 1.0;
			VOHIter voh_it;
			for (voh_it = voh_iter(orderedInsideVertex[i]); voh_it; ++voh_it) {
				bool find = false;
				VertexHandle _vh = to_vertex_handle(voh_it);
				for (int j = 0; j < orderedInsideVertex.size(); ++j) {
					if (_vh == orderedInsideVertex[j]) {
						A.insert(i, j) = -property(weight, voh_it);
						find = true;
						break;
					}
				}
				for (int j = 0; j < orderedOutsideVertex.size(); ++j) {
					if (find) {
						break;
					}
					if (_vh == orderedOutsideVertex[j]) {
						OpenMesh::Vec2d pos = property(newPos, orderedOutsideVertex[j]);
						std::cout << "get outsider vertex " << j << " :" << pos[0] << " " << pos[1] << "\n";
						std::cout << "show weight:" << property(weight, voh_it) << "\n";
						B[0][i] += property(weight, voh_it) * pos[0];
						B[1][i] += property(weight, voh_it) * pos[1];
						break;
					}
				}
			}
		}
		A.makeCompressed();
		Eigen::SparseQR<Eigen::SparseMatrix<double>, Eigen::COLAMDOrdering<int>> linearSolver;
		linearSolver.compute(A);
		std::vector<Eigen::VectorXd> X;
		X.resize(2);
		X[0] = linearSolver.solve(B[0]);
		X[1] = linearSolver.solve(B[1]);

		std::cout << "show A matrix:\n";
		for (int i = 0; i < orderedInsideVertex.size(); ++i) {
			for (int j = 0; j < orderedInsideVertex.size(); ++j) {
				std::cout << A.coeff(i, j) << " ";
			}
			std::cout << "\n";
		}
		std::cout << "show matrix B:\n";
		for (int j = 0; j < orderedInsideVertex.size(); ++j) {
			std::cout << B[0][j] << " " << B[1][j] << "\n";
		}
		
		std::cout << "show matrix X:\n";
		for (int j = 0; j < orderedInsideVertex.size(); ++j) {
			std::cout << X[0][j] << " " << X[1][j] << "\n";
		}

		insideVertexData.clear();
		for (int i = 0; i < orderedInsideVertex.size(); ++i) {
			OpenMesh::Vec2d pos;
			pos[0] = X[0][i];
			pos[1] = X[1][i];
			property(newPos, orderedInsideVertex[i]) = pos;
			insideVertexData.push_back(pos[0]);
			insideVertexData.push_back(pos[1]);
			insideVertexData.push_back(0);
		}
	}
	// 重組新座標
	solvedData.clear();
	for (int offset : chosenFace)
	{
		FaceHandle _fh = face_handle(offset);
		FVIter fv_it;
		for (fv_it = fv_iter(_fh); fv_it; ++fv_it)
		{
			VertexHandle _vh = fv_it.handle();
			OpenMesh::Vec2d pos = property(newPos, _vh);
			solvedData.push_back(pos[0]);
			solvedData.push_back(pos[1]);
			solvedData.push_back(0);
		}
	}
	//準備當外框的圓圈
	for (float i = 0; i < 1; i += 0.01f) {
		circleData.push_back(i);
		circleData.push_back(0);
		circleData.push_back(0);
	}
	for (float i = 0; i < 1; i += 0.01f) {
		circleData.push_back(1);
		circleData.push_back(i);
		circleData.push_back(0);
	}
	for (float i = 0; i < 1; i += 0.01f) {
		circleData.push_back(1-i);
		circleData.push_back(1);
		circleData.push_back(0);
	}
	for (float i = 0; i < 1; i += 0.01f) {
		circleData.push_back(0);
		circleData.push_back(1-i);
		circleData.push_back(0);
	}
	//準備外圈座標用來畫的
	/*for (int i = 0; i < orderedOutsideVertex.size() - 1; ++i) {
		OpenMesh::Vec2d curPos = property(newPos, orderedOutsideVertex[i]);
		OpenMesh::Vec2d nxtPos = property(newPos, orderedOutsideVertex[i + 1]);
		if (curPos[0] < nxtPos[0]) {
			textureData.push_back(curPos[0]);
			textureData.push_back(curPos[1]);
			textureData.push_back(zPos);

			textureData.push_back(nxtPos[0]);
			textureData.push_back(nxtPos[1]);
			textureData.push_back(zPos);

			textureData.push_back(0);
			textureData.push_back(0);
			textureData.push_back(zPos);
		}
		else {
			textureData.push_back(nxtPos[0]);
			textureData.push_back(nxtPos[1]);
			textureData.push_back(zPos);

			textureData.push_back(curPos[0]);
			textureData.push_back(curPos[1]);
			textureData.push_back(zPos);

			textureData.push_back(0);
			textureData.push_back(0);
			textureData.push_back(zPos);
		}
	}*/

}
void Tri_Mesh::Render_Wireframe()
{
	//glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	
	glColor3f(0.0, 0.0, 0.0);	

	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(curVertex.data());			
	}
	glEnd();
	
}

void Tri_Mesh::Render_Point()
{
	glPointSize ( 8.0 ) ;				  
	glColor3f( 1.0, 0.0, 0.0 ) ;
	glBegin(GL_POINTS);
	for (OMT::VIter v_it = vertices_begin() ; v_it != vertices_end() ; ++v_it)
	{
		  glVertex3dv(point(v_it).data());
	}
	glEnd();
}

bool ReadFile(std::string _fileName,Tri_Mesh *_mesh)
{
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if ( OpenMesh::IO::read_mesh(*_mesh, _fileName, opt) )
	{
		//read mesh from filename OK!
		isRead = true;
	}
	if(isRead)
	{
		// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
		if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && _mesh->has_vertex_normals())
		{
			_mesh->update_normals();
		}
	}
	return isRead;
}

bool SaveFile(std::string _fileName, Tri_Mesh *_mesh)
{
	bool isSave = false;
	if (OpenMesh::IO::write_mesh(*_mesh, _fileName))
	{
		isSave = true;
	}
	return isSave;
}
