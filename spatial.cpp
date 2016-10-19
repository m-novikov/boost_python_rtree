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
            py::list nearest(GeoPoint point) {
                std::vector<Value> result;
                rtree.query(bgi::nearest(point, 5), std::back_inserter(result));
                py::list ret;
                for (Value& value : result)
                    ret.append(py::make_tuple(value.first, value.second));
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
    ;
    py::class_<Value>("Value", py::init<GeoPoint, py::object>());
}
