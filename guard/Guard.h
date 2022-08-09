#ifndef GUARD_H_INCLUDED
#define GUARD_H_INCLUDED

class GuardBase
{
public:
    void dismiss() const {dismissed = true;}
protected:
    GuardBase() : dismissed(false) {}
    ~GuardBase() {}
    mutable bool dismissed;
};

template <typename Object, typename Deleter>
class Guard_o : public GuardBase {
public:
    Guard_o(const Object &obj, const Deleter &del) : obj(obj), d(del) {}
    ~Guard_o(){
        if (!dismissed) d(obj);
    }
private:
    const Object &obj;
    const Deleter *d;
};

template <typename Object, typename Deleter>
Guard_o<Object, Deleter> make_guard(const Object &obj, const Deleter &d)
{
    return Guard_o<Object, Deleter>(obj, d);
}

template <typename Deleter>
class Guard : public GuardBase {
public:
    Guard(const Deleter &del) : d(del) {}
    ~Guard(){
        if (!dismissed) d();
    }
private:
    const Deleter d;
};

template <typename Deleter>
Guard<Deleter> make_guard(const Deleter &d)
{
    return Guard<Deleter>(d);
}

template <typename Object, typename Deleter>
class Guard_m : public GuardBase {
public:
    Guard_m(Object &obj, const Deleter &del) : obj(obj), d(del) {}
    ~Guard_m(){
        if (!dismissed) (obj.*d)();
    }
private:
    Object &obj;
    const Deleter d;
};

template <typename Object, typename Deleter>
class Guard_m<Object *, Deleter> : public GuardBase {
public:
    Guard_m(Object *obj, const Deleter &del) : obj(obj), d(del) {}
    ~Guard_m(){
        if (!dismissed) (obj->*d)();
    }
private:
    Object *obj;
    const Deleter d;
};

template <typename Object, typename Deleter>
Guard_m<Object, Deleter> make_guard_m(Object &obj, const Deleter &d)
{
    return Guard_m<Object, Deleter>(obj, d);
}

using guard = const GuardBase &;

#endif // GUARD_H_INCLUDED
