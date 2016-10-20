#include <vector>
#include <utility>
#include <sstream>

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
namespace py = boost::python;

typedef bg::model::d2::point_xy <double, bg::cs::spherical_equatorial<bg::degree>> GeoPoint;
typedef std::pair<GeoPoint, py::object> Value;
typedef bgi::rtree<Value, bgi::rstar<16>> RTree;
typedef bg::strategy::distance::haversine<double> haversine_type;

namespace {
    class PythonRTree {
        public:
            PythonRTree() {}
            PythonRTree(py::object obj) {
                py::stl_input_iterator<Value> begin(obj), end;
                std::vector<Value> data(begin, end);
                this->rtree = RTree(data.begin(), data.end());
            }
            void insert(Value value) {
                rtree.insert(value);
            }
            py::list nearest(GeoPoint point, unsigned num) {
                std::vector<Value> result;
                py::list ret;
                for (auto it = rtree.qbegin(bgi::nearest(point, num)); it != rtree.qend(); ++it ) {
                    ret.append(py::make_tuple((*it).first, (*it).second));
                }
                return ret;
            }
            py::list nearest_with_distance(GeoPoint point, unsigned num, double max_distance) {
                std::vector<Value> result;
                py::list ret;
                for (auto it = rtree.qbegin(bgi::nearest(point, num)); it != rtree.qend(); ++it ) {
                    if (bg::distance((*it).first, point, haversine_type()) > max_distance) {
                        break;
                    }
                    ret.append(py::make_tuple((*it).first, (*it).second));
                }
                return ret;
            }
        private:
            RTree rtree;
    };
    std::string point_str(const GeoPoint& point) {
        std::stringstream ss;
        ss << bg::wkt(point);
        return ss.str();
    }
}

BOOST_PYTHON_MODULE(spatial_index)
{
    py::class_<GeoPoint>("GeoPoint", py::init<double, double>())
        .def("__str__", point_str)
    ;
    py::class_<PythonRTree>("RTree", py::init<>())
        .def(py::init<py::object>())
        .def("insert", &PythonRTree::insert)
        .def("nearest", &PythonRTree::nearest)
        .def("nearest", &PythonRTree::nearest_with_distance)
    ;

    py::class_<Value>("Value", py::init<GeoPoint, py::object>());
}
