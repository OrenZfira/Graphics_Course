#version 330 

uniform vec4 eye;
uniform vec4 ambient;
uniform vec4[20] objects;
uniform vec4[20] objColors;
uniform vec4[10] lightsDirection;
uniform vec4[10] lightsIntensity;
uniform vec4[10] lightsPosition;
uniform float theta;
uniform float phi;
uniform ivec4 sizes;//total amount of object, number of lights, reflective objects, transperent objects
uniform mat4 Model;
in vec3 position0;
in vec3 normal0;

float intersection(inout int index, vec3 source, vec3 v){
    float tmin = 1.0e10;
    int indx = -1;
    for (int j = 0; j<5; j++){
        for(int i = 0; i<sizes.x; i++){
            if(i == index)
                continue;
            float t = -1;
            if (objects[i].w <= 0 ) { //plane
                vec3 n = normalize(objects[i].xyz);
                t = -(dot(source.xyz,n) + objects[i].w)/(dot(v,n));
            }
            else { //sphere
                vec3 p0O = source - objects[i].xyz;
                float b = dot(v,p0O);
                float c = dot(p0O,p0O)-objects[i].w*objects[i].w;
                float d = b*b-c;
                if (d>=0){
                    float t1 = -b + sqrt(d);
                    float t2 = -b - sqrt(d);
                    t = min(t1,t2);
                    if (t < 0)
                        t=max(t1,t2);
                }
            }
            if (t < tmin && t >=0) {
                tmin = t;
                indx = i;
            }
        }
        break;
        if(indx >= sizes.w || indx == -1) break;
        float t=-1;
        vec3 n = normalize(objects[indx].xyz - source);
        float theta1 = acos(dot(n, -v));
        float theta2 = asin(sin(theta1)/1.5);
        source = source + tmin*v;               //new source from where we intersect with the sphere
        v = (2.0/3.0 * cos(theta1) - cos(theta2))*n - 2.0/3.0*-v;       //finding new v using snell's law

        //finding intersection with other side of sphere
        vec3 p0O = source - objects[indx].xyz;
        float b = dot(v,p0O);
        float c = dot(p0O,p0O)-objects[indx].w*objects[indx].w;
        float d = b*b-c;
        if (d>=0){//maybe not needed
            float t1 = -b + sqrt(d);
            float t2 = -b - sqrt(d);
            t=max(t1,t2);
            n = normalize(objects[indx].xyz - source);
            theta1 = acos(dot(-n, -v));
            theta2 = asin(sin(theta1)/1.5);
            source = source + t*v; // new source from final intersection with sphere
            v = (1.5 * cos(theta1) - cos(theta2))*-n - 1.5*-v;  //vector coming out of the sphere
        }
        // n = normalize(objects[indx].xyz - source);
        // theta1 = acos(dot(-n, -v));
        // theta2 = asin(sin(theta1)/1.5);
        // source = source + t*v; // new source from final intersection with sphere
        // v = (1.5 * cos(theta1) - cos(theta2))*-n - 1.5*-v;  //vector coming out of the sphere
    }
    index = indx;
    return tmin;
}

vec3 snell(int index, inout vec3 dest, vec3 v){
    vec3 sphere_norm = normalize(dest - objects[index].xyz);
    float theta_in = acos(dot(sphere_norm, -v));
    float theta_out = asin(sin(theta_in) / 1.5);
    vec3 into_sphere = (2/3*cos(theta_in) - cos(theta_out)) * sphere_norm + 2/3 * v;
    float t = 0;

    float b = dot(into_sphere, dest);
    float c = dot(dest,dest)-objects[index].w*objects[index].w;
    float d = b*b-c;
    if (d>=0){
        float t1 = -b + sqrt(d);
        float t2 = -b - sqrt(d);
        t = max(t1, t2);
        // t = min(t1,t2);
        // if (t <= 0)
        //     t=max(t1,t2);
    }
    dest += into_sphere*t;
    sphere_norm = -normalize(dest - objects[index].xyz);
    theta_in = acos(dot(sphere_norm, -into_sphere));
    theta_out = asin(sin(theta_in) * 1.5);
    return (1.5*cos(theta_in)-cos(theta_out))*sphere_norm + 1.5 * into_sphere;
}


vec3 phong(int index, vec3 source, vec3 v, float factor){
    vec3 color = ambient.rgb*objColors[index].rgb;
    float shine = objColors[index].w;
    vec3 diff = objColors[index].rgb * factor;
    float spec = 0.7;
    vec3 normal;
    
    if (objects[index].w <= 0)
        normal = normalize(objects[index].xyz);
    else
        normal = normalize(objects[index].xyz - source);//-normalize(source - objects[index].xyz);
    
    for(int i = 0; i<sizes.y; i++){
        if (lightsDirection[i].w < 1 ){
            int indx = index;
            vec3 l = normalize(lightsDirection[i].xyz);
            float t = intersection(indx, source, -l);
            if (indx < 0 || objects[indx].w <= 0){ //checking for intersection
                vec3 r = normalize(reflect(l,normal));
                color += max(vec3(0.0, 0.0, 0.0), (diff*(dot(normal,l)*lightsIntensity[i].rgb)));//diffuse
                if( objects[index].w <= 0 ||dot(l, normal)>0.0)
                    color += max( vec3(0.0, 0.0, 0.0), spec*(pow(dot(v,r), shine)*lightsIntensity[i].rgb));//specular
                if(objects[index].w <= 0){
                    color = min(vec3(1.0,1.0,1.0), color);
                }
            }
        }
        else{
            int indx = -1;
            vec3 l = normalize(source - lightsPosition[i].xyz);//vector from the light source to the object
            
            if(dot(l, normalize(lightsDirection[i].xyz)) <  lightsPosition[i].w){
                continue;}
            else {
                float t = intersection(indx, lightsPosition[i].xyz, l);
                if (indx == index){
                    vec3 r = normalize(reflect(l,normal));
                    color += max(vec3(0.0, 0.0, 0.0), (diff*(dot(normal,l)*lightsIntensity[i].rgb)));//diffuse
                    if(objects[index].w <= 0 || dot(l, normal)>0)
                        color += max( vec3(0.0, 0.0, 0.0), spec*(pow(dot(v,r), shine)*lightsIntensity[i].rgb));//specular
                    if(objects[index].w <= 0){
                        color = min(vec3(1.0,1.0,1.0), color);
                    }
                }
                
            }
        }
        
    }
    return color;
}

void main(){
    vec4 eye1 = Model*eye;
    vec3 v = normalize(position0 + eye1.xyw - eye1.xyz);
    int index = -1;
    float t = intersection(index,position0 + eye1.xyw ,v);
    if (index >= 0){
        vec3 p = position0 + eye1.xyw + t*v;
        vec3 normal;
        for(int i = 0;i < 5 && index < sizes.z+sizes.w-0.1f; i++){
            if (index >= 0 && index <sizes.w -0.1f){
                v = snell(index,p,v);
            }
            if(objects[index].w <= 0)
                normal = normalize(objects[index].xyz);
            else
                normal = normalize(p - objects[index].xyz);
            v = normalize(reflect(v,normal));
            t = intersection(index, p, v);
            p += t*v;
            
        }
        
        float x = p.x;
        float y = p.y;
        if(objects[index].w <= 0 && (((mod(int(1.5*x),2) == mod(int(1.5*y),2)) && ((x>0 && y>0) || (x<0 && y<0))) || ((mod(int(1.5*x),2) != mod(int(1.5*y),2) && ((x<0 && y>0) || (x>0 && y<0))))))
            gl_FragColor = vec4(phong(index,p,v,0.5),1);
        else 
            gl_FragColor = vec4(phong(index,p,v,1.0),1);      
    }
}