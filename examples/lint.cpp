
#include <verse.h>

#include <Ample.h>

using namespace verse::ample;

class Listener : public SessionObserver,
		 //public TextNodeObserver,
		 public TextBufferObserver,
		 public GeometryNodeObserver,
		 public GeometryLayerObserver,
		 public TagGroupObserver,
		 public TagObserver
{
public:
  void onAccept(Session& session)
  {
    printf("Accepted session %s\n", session.getAddress().c_str());
    session.createNode("text", V_NT_TEXT);
    session.createNode("geometry", V_NT_GEOMETRY);
  }

  void onTerminate(Session& session, const std::string& byebye)
  {
    printf("Terminated session %s with %s\n", session.getAddress().c_str(), byebye.c_str());
  }

  void onCreateNode(Session& session, Node& node)
  {
    printf("Created node %u of type %u\n", node.getID(), node.getType());
    if (node.isMine())
    {
      node.addObserver(*this);
      node.createTagGroup("group");
      switch (node.getType())
      {
	case V_NT_GEOMETRY:
	{
	  GeometryNode* geometryNode = dynamic_cast<GeometryNode*>(&node);
	  geometryNode->createLayer("layer", VN_G_LAYER_VERTEX_REAL);
	  break;
	}

	case V_NT_TEXT:
	{
	  TextNode* textNode = dynamic_cast<TextNode*>(&node);
	  textNode->createBuffer("buffer");
	  break;
	}
      }
    }
  }

  void onDestroyNode(Session& session, Node& node)
  {
    printf("Destroyed node %u (%s)\n", node.getID(), node.getName().c_str());
  }

  void onCreateTagGroup(Node& node, TagGroup& group)
  {
    printf("Created tag group %s in node %u (%s)\n", group.getName().c_str(), node.getID(), node.getName().c_str());
    group.addObserver(*this);
    VNTag value;
    value.vboolean = true;
    group.createTag("tag", VN_TAG_BOOLEAN, value);
  }

  void onDestroyTagGroup(Node& node, TagGroup& group)
  {
    printf("Destroyed tag group %s in node %u (%s)\n", group.getName().c_str(), node.getID(), node.getName().c_str());
  }

  void onSetName(Node& node, const std::string& name)
  {
    printf("Set name %s for node %u\n", name.c_str(), node.getID());
  }

  void onCreateLayer(GeometryNode& node, GeometryLayer& layer)
  {
    printf("Created geometry layer %s of type %u in node %u (%s)\n",
           layer.getName().c_str(), layer.getType(), node.getID(), node.getName().c_str());
    if (node.isMine())
    {
      layer.addObserver(*this);
      layer.setName("renamed_layer");
    }
  }

  void onDestroyLayer(GeometryNode& node, GeometryLayer& layer)
  {
    printf("Destroyed geometry layer %s of in node %u (%s)\n",
           layer.getName().c_str(), node.getID(), node.getName().c_str());
  }

  void onSetSlot(GeometryLayer& layer, uint16 slotID, const void* data)
  {
    printf("Changed slot %u in layer %s of node %u (%s)\n", slotID, layer.getName().c_str(), layer.getNode().getID(),
	   layer.getNode().getName().c_str());
  }
  
  void onSetName(GeometryLayer& layer, const std::string& name)
  {
    printf("Changed name of layer %s to %s in node %u (%s)\n", layer.getName().c_str(), name.c_str(), layer.getNode().getID(),
	   layer.getNode().getName().c_str());
  }

  void onCreateTag(TagGroup& group, Tag& tag)
  {
    printf("Created tag %s in group %s of node %u (%s)\n", tag.getName().c_str(), group.getName().c_str(),
	   group.getNode().getID(), group.getNode().getName().c_str());
    tag.addObserver(*this);
    tag.setName("zyxel");
  }

  void onDestroyTag(TagGroup& group, Tag& tag)
  {
    printf("Destroyed tag %s in group %s of node %u (%s)\n", tag.getName().c_str(), group.getName().c_str(),
	   group.getNode().getID(), group.getNode().getName().c_str());
  }

  void onSetName(TagGroup& group, const std::string& name)
  {
    printf("Changed name of group %s to %s of node %u (%s)\n", group.getName().c_str(), name.c_str(), group.getNode().getID(), group.getNode().getName().c_str());
  }

  void onChange(Tag& tag)
  {
    printf("Changed something in tag %s in group %s of node %u (%s)\n", tag.getName().c_str(), tag.getGroup().getName().c_str(),
    tag.getGroup().getNode().getID(), tag.getGroup().getNode().getName().c_str());
  }
};

int main(int argc, char** argv)
{
  std::string host;

  if (argc == 1)
    host = "localhost";
  else
    host = argv[1];

  Session* session = Session::create(host, "spoo", "fleem");
  if (!session)
    return 1;

  Listener listener;
  session->addObserver(listener);

  while (session->getState() != Session::TERMINATED)
    Session::update(10000);
}

