import Foundation

let inputJSON = """
{
	"candidates" : [
		{
			"geometry" : {
				"location" : {
					"lat" : 48.470247,
					"lng" : 35.036206				
				}
			},
			"name" : "Coffee Life"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.469434,
					"lng" : 35.038985				
				}
			},
			"name" : "Have Coffee"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.466561,
					"lng" : 35.045186				
				}
			},
			"name" : "AROMA KAVA"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.463783,
					"lng" : 35.042595				
				}
			},
			"name" : "Mr. Coffee"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.463516,
					"lng" : 35.048889				
				}
			},
			"name" : "DoubleDecker"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.425694,
					"lng" : 35.025487				
				}
			},
			"name" : "Mocco bar"
		},
		{
			"geometry" : {
				"location" : {
					"lat" : 48.451214,
					"lng" : 35.057789				
				}
			},
			"name" : "Confetti"
		},
		{
         		"geometry" : {
            			"location" : {
               				"lat" : 48.460239,
               				"lng" : 35.069301
            			}
         		},
         		"name" : "One dollar bar"
      		},
      		{
         		"geometry" : {
            			"location" : {
               				"lat" : 48.459414,
               				"lng" : 35.064731
            			}
         		},
         		"name" : "Heisenberg Coffee Lab"
      		},
      		{
         		"geometry" : {
            			"location" : {
               				"lat" : 48.423311,
               				"lng" : 35.058121
            			}
         		},
         		"name" : "Sopranos"
      		}]
}
"""


struct Places: Codable
{
	let candidates: [Candidate]		
}
struct Candidate: Codable
{
	let geometry: Geometry
	let name: String
}

struct Geometry: Codable
{
	let location: Location
}
struct Location: Codable
{
	let lat: Double
	let lng: Double
}

class CoolPlacesNearMe
{
	private var places: Places
	private var distances: [Double] = []
	private var base: Location
	private var radius: Double
	
	init(json : String, base: Location, radius: Double)
	{
		let inputData = json.data(using: .utf8)!
		let decoder = JSONDecoder()
		places = try! decoder.decode(Places.self, from: inputData)
		self.base = base
		self.radius = radius
		for place in places.candidates {
			distances.append(distance(p1: place.geometry.location, p2: base))		
		}
	}
	func distance(p1: Location, p2: Location) -> Double
	{
		let rad = {(deg: Double) -> Double in return deg * (.pi/180)}
		let R = 6371.0
  		let dLat = rad(p2.lat - p1.lat)
  		let dLon = rad(p2.lng - p1.lng) 
  		let a = sin(dLat/2) * sin(dLat/2) + cos(rad(p1.lat)) * cos(rad(p2.lat)) * sin(dLon/2) * sin(dLon/2) 
  		let c = 2.0 * atan2(sqrt(a), sqrt(1-a))
  		let d = R * c
  		
		return d		
	}
	func nearPlaces() -> [(name: String, dist: Double)]
	{
		var namesAndDistances: [(name: String, dist: Double)] = []
		for(place, distance) in zip(places.candidates, distances) {
			if(distance <= radius) {
				namesAndDistances.append((name: place.name, dist: distance))
			}				
		}
		
		return namesAndDistances
	}
}


var baseApriorit = Location(lat: 48.471207, lng: 35.038810)
var nearApriorit = CoolPlacesNearMe(json: inputJSON, base: baseApriorit, radius: 1)
let namesAndDistances = nearApriorit.nearPlaces()
for (name, dist) in namesAndDistances {
	print(String(format: "Name: %@, distance: %.3f", name, dist))
}



